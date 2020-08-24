struct vOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

vOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	vOut output;

	output.position = position;
	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}