#pragma once
#include "WGPUFramework.h"
#include "RenderPipelineBuilder.h"
#include "DcmReader.h"

namespace DCR {

	class Application
	{
		struct UniformStruct {
			int32_t slice = 0;
		};

		UniformStruct m_uniforms;

	public:
		Application();

		void Run();

		~Application();

		void OnFileDrop(FileDropEvent& e);

		void KeyboardListener(KeyboardEvent& e);

	private:
		Texture3D m_volumeImage;
		itk::Size<3> m_size;

		Buffer m_uniformBuffer;
		Buffer m_square;
		Buffer m_squareIndex;

		RenderPipelineBuilder m_pipelineBuilder = WGF::CreateRenderPipelineBuilder();
		RenderPipeline m_pipeline;

		WGPUBindGroupDescriptor m_bindGroupDesc = {};
		WGPUBindGroup m_bindGroup = nullptr;
		std::vector<WGPUBindGroupEntry> m_bindingEntries = std::vector<WGPUBindGroupEntry>(2);
	};
}
