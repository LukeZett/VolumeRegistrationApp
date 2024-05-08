struct VertexInput {
    @location(0) position: vec2f,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) texC: vec2f,
};

@group(0) @binding(0) var<uniform> slice: i32;
@group(0) @binding(1) var volumeTexture: texture_3d<f32>;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = vec4f(in.position, 0.0, 1.0);
    out.texC = vec2f(in.position * 0.5 + 0.5);
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let texCoords = vec2i(in.texC * vec2f(textureDimensions(volumeTexture).xy));
    let volCoords = vec3i(texCoords, slice);
    let colorTex = textureLoad(volumeTexture, volCoords, 0);
    return vec4f(colorTex.rrr, 1.0);
}
