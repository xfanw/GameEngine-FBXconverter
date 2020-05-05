#version 430 core

// ------------------------------------------
//    Compute World Matrix
// ------------------------------------------

// localize breakdown
layout(local_size_x = 20) in;


struct Bone
{
	vec4 t;
	vec4 q;
	vec4 s;
};


layout (std430, binding = 0) buffer LocalBone
{
	Bone LocalBone_array[];
};

layout (std430, binding = 1) buffer HierarchyTable
{
	int HierarchyTable_Entry[];
};

layout (std430, binding = 2) buffer Result
{
	mat4 Result_array[];
};


// Uniforms
uniform int boneCount;
uniform int hierarchyDepth;

// FRANK
uniform mat4 rigidWorld;

// Function
mat4 Bone2Mat (Bone bone);
mat4 TransMatrix(vec4 trans);
mat4 ScaleMatrix(vec4 scale);
mat4 RotMatrix(vec4 quat);

void main()
{
	uint boneIndex = gl_GlobalInvocationID.x;

	if( boneIndex < boneCount )
	{
		// identity 
		//mat4 tmp = mat(1.0);
		mat4 tmp = rigidWorld;
		int d = HierarchyTable_Entry[boneIndex * hierarchyDepth];

		for(int col = 1; col < d ; col++){
				// parentindex <- Table(Row + offset)
			int currCol = HierarchyTable_Entry[(boneIndex * hierarchyDepth) + col];
				// mutiply in reverse order ...
			tmp = tmp * Bone2Mat(LocalBone_array[currCol]);
		}
		
		Result_array[boneIndex] = tmp;
	}
	
};

mat4 Bone2Mat(Bone bone){
	mat4 mTrans = TransMatrix(bone.t);
	mat4 mScale = ScaleMatrix(bone.s);
	mat4 mRot = RotMatrix(bone.q);

	mat4 result;
	result = mTrans * mRot * mScale ;

	return result;

}

mat4 TransMatrix(vec4 trans){
	mat4 result = mat4(1.0);
	result[3] = vec4(trans.xyz, 1.0);

	return result;
}
mat4 ScaleMatrix(vec4 scale){
	mat4 result = mat4(1.0);
	result[0].x = scale.x;
	result[1].y = scale.y;
	result[2].z = scale.z;

	return result;
}
mat4 RotMatrix(vec4 quat){
	mat4 result = mat4(1.0);

	result[0].x = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
	result[0].y = 2 * (quat.x * quat.y + quat.w * quat.z);
	result[0].z = 2 * (quat.x * quat.z - quat.w * quat.y);
	result[0].w = 0.0;

	result[1].x = 2 * (quat.x * quat.y - quat.w * quat.z);
	result[1].y = 1 - 2 * (quat.x * quat.x + quat.z * quat.z);
	result[1].z = 2 * (quat.y * quat.z + quat.w * quat.x);
	result[1].w = 0.0;

	result[2].x = 2 * (quat.x * quat.z + quat.w * quat.y);
	result[2].y = 2 * (quat.y * quat.z - quat.w * quat.x);
	result[2].z = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
	result[2].w = 0.0;

	result[3] = vec4(0.0, 0.0, 0.0, 1.0);

	return result;

}