#pragma once
#include "Matrix.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <math.h>

struct Vector2 {
	float num[2];
};

struct Vector3 {
	float num[3];
};

struct Vector4 {
	float num[4];
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct SpriteData {
	Vector4 positionLeftTop[2];
	Vector4 positionRightDown[2];
	Vector4 material;
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};