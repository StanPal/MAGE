#pragma once
namespace MAGE::Graphics
{
	class ConstantBuffer
	{
	public:
		void Initialize(uint32_t bufferSize);
		void Terminate();
		//VertexShader
		void BindVS(uint32_t slot = 0) const;
		//PixelShader
		void BindPS(uint32_t slot = 0) const;

		void Update(const void* data);
	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
		float mRotation = 0.0f;
	};

	template<class T>
	class TypedConstantBuffer : public ConstantBuffer
	{
	public:
		using DataType = T;

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0,
				"[TypedConstantBuffer] data type is must be 16 byte aligned.");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Update(const DataType& data) 
		{
			ConstantBuffer::Update(&data);
		}
	};
}