#include "Application.h"
#include "ImageBuffer.h"

using namespace DCR;

Application::Application() : m_square(sizeof(float) * 8, 0, Vertex),
	m_squareIndex(sizeof(uint16_t) * 6, 0, Index),
	m_uniformBuffer(sizeof(UniformStruct), 0, Uniform)
{
	WGF::Window().SetFileDropCallback(FILEDROP_CALLBACK(Application::OnFileDrop, *this));
	WGF::Window().SetKeyCallback(KEYBOARD_CALLBACK(Application::KeyboardListener, *this));

	std::vector<float> verts{
		// x,   y,
		-1.0, -1.0,
		+1.0, -1.0,
		+1.0, +1.0,
		-1.0, +1.0
	};

	std::vector<uint16_t> indices{
		0, 1, 2,
		0, 2, 3
	};

	m_square.Upload(verts);
	m_square.GetLayout().AddElement<float>(2);

	m_squareIndex.Upload(indices);

	m_volumeImage.GetDescriptor().format = WGPUTextureFormat_R8Unorm;
	m_volumeImage.GetViewDesc().format = WGPUTextureFormat_R8Unorm;

	m_volumeImage.Init(100, 100, 100);

	m_pipeline = m_pipelineBuilder.SetShader("SliceView.wgsl")
		.AddTextureBinding(0, 1, FragmentShader, WGPUTextureViewDimension_3D)
		.AddUniformBinding<UniformStruct>(0, 0, FragmentShader)
		.AddBufferLayout(m_square.GetLayout())
		.Build();

	m_uniformBuffer.Upload(&m_uniforms, 1);
}

void Application::Run()
{
	auto& uniforms = m_bindingEntries[0];
	uniforms.nextInChain = nullptr;
	uniforms.binding = 0;
	uniforms.buffer = m_uniformBuffer.Get();
	uniforms.offset = 0;
	uniforms.size = sizeof(UniformStruct);

	auto& tex =  m_bindingEntries[1];

	tex.nextInChain = nullptr;
	tex.binding = 1;
	tex.textureView = m_volumeImage.GetView();

	m_bindGroupDesc.nextInChain = nullptr;
	m_bindGroupDesc.layout = m_pipeline.GetBindGroupLayout(0);

	m_bindGroupDesc.entryCount = m_pipeline.GetBindingCount(0);
	m_bindGroupDesc.entries = &m_bindingEntries[0];
	m_bindGroup = wgpuDeviceCreateBindGroup(Device::Get(), &m_bindGroupDesc);
	
	while (!WGF::Window().ShouldClose())
	{
		auto encoder = WGF::BeginScreenPass();

		wgpuRenderPassEncoderSetPipeline(encoder, m_pipeline.Get());
		wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, m_square.Get(), 0, m_square.Size());
		wgpuRenderPassEncoderSetIndexBuffer(encoder, m_squareIndex.Get(), WGPUIndexFormat_Uint16, 0, 6 * 2);
		wgpuRenderPassEncoderSetBindGroup(encoder, 0, m_bindGroup, 0, 0);
		wgpuRenderPassEncoderDrawIndexed(encoder, 6, 1, 0, 0, 0);

		WGF::FinishScreenPass();
		WGF::Window().PollEvents();
	}
	wgpuBindGroupRelease(m_bindGroup);
}

Application::~Application()
{
}

void Application::OnFileDrop(FileDropEvent& e)
{

	auto image = DcmReader(e.filePaths[0]);
	auto imgbuffer = ImageBuffer<int16_t, 3, uint8_t>(image.GetOutput());
	m_size = imgbuffer.GetSize();

	m_volumeImage.Init(m_size[0], m_size[1], m_size[2]);
	m_volumeImage.Write(imgbuffer.GetData(), { 0,0,0 }, { m_size[0], m_size[1], m_size[2]}, sizeof(uint8_t));
	m_uniforms.slice = 0;
	m_uniformBuffer.Upload(&m_uniforms, 1);

	m_bindingEntries[1].textureView = m_volumeImage.GetView();

	wgpuBindGroupRelease(m_bindGroup);
	m_bindGroup = wgpuDeviceCreateBindGroup(Device::Get(), &m_bindGroupDesc);
	
}

void Application::KeyboardListener(KeyboardEvent& e)
{
	if (e.action == GLFW_RELEASE)
	{
		return;
	}

	if (e.button ==  GLFW_KEY_K)
	{
		m_uniforms.slice++;
		m_uniforms.slice = glm::min((int32_t)m_size[2] - 1, m_uniforms.slice);
	}
	if (e.button == GLFW_KEY_L)
	{
		m_uniforms.slice--;
		m_uniforms.slice = glm::max(0, m_uniforms.slice);
	}

	m_uniformBuffer.Upload(&m_uniforms, 1);

}
