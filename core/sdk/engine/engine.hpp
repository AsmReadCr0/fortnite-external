#pragma once
#include <d3d9.h>

#include <core/includes.hpp>

#include <dependencies/driver/driver.hpp>

namespace uengine
{
	struct fquat {
		float w, x, y, z;

		fquat() : w(1), x(0), y(0), z(0) {}

		fquat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
	};

	struct ftransform
	{
		uemath::fplane rot;
		uemath::fvector translation;
		char pad_38[4];
		uemath::fvector scale = { 1.0, 1.0, 1.0 };
		char pad_58[4];

		inline D3DMATRIX to_matrix_with_scale() const
		{
			D3DMATRIX m{};
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			auto x2 = rot.x + rot.x, y2 = rot.y + rot.y, z2 = rot.z + rot.z;
			auto xx2 = rot.x * x2, yy2 = rot.y * y2, zz2 = rot.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale.z;

			auto yz2 = rot.y * z2, wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * scale.z;
			m._23 = (yz2 + wx2) * scale.y;

			auto xy2 = rot.x * y2, wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * scale.y;
			m._12 = (xy2 + wz2) * scale.x;

			auto xz2 = rot.x * z2, wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * scale.z;
			m._13 = (xz2 - wy2) * scale.x;

			m._14 = m._24 = m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

	inline D3DMATRIX matrix_multiplication(const D3DMATRIX& matrix1, const D3DMATRIX& matrix2)
	{
		D3DMATRIX result{};

		result._11 = matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41;
		result._12 = matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42;
		result._13 = matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43;
		result._14 = matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44;

		result._21 = matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41;
		result._22 = matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42;
		result._23 = matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43;
		result._24 = matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44;

		result._31 = matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41;
		result._32 = matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42;
		result._33 = matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43;
		result._34 = matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44;

		result._41 = matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41;
		result._42 = matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42;
		result._43 = matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43;
		result._44 = matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44;

		return result;
	}

	inline D3DMATRIX create_rotation_matrix(const uemath::frotator& rotation)
	{
		auto rad_x = rotation.pitch * float(std::numbers::pi) / 180.f;
		auto rad_y = rotation.yaw * float(std::numbers::pi) / 180.f;
		auto rad_z = rotation.roll * float(std::numbers::pi) / 180.f;

		auto sp = sinf(rad_x), cp = cosf(rad_x);
		auto sy = sinf(rad_y), cy = cosf(rad_y);
		auto sr = sinf(rad_z), cr = cosf(rad_z);

		D3DMATRIX matrix{};
		matrix.m[0][0] = cp * cy;
		matrix.m[0][1] = cp * sy;
		matrix.m[0][2] = sp;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = sr * sp * cy - cr * sy;
		matrix.m[1][1] = sr * sp * sy + cr * cy;
		matrix.m[1][2] = -sr * cp;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(cr * sp * cy + sr * sy);
		matrix.m[2][1] = cy * sr - cr * sp * sy;
		matrix.m[2][2] = cr * cp;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	struct alignas(16) matrix_elements {
		double m11, m12, m13, m14;
		double m21, m22, m23, m24;
		double m31, m32, m33, m34;
		double m41, m42, m43, m44;

		matrix_elements() : m11(0), m12(0), m13(0), m14(0),
			m21(0), m22(0), m23(0), m24(0),
			m31(0), m32(0), m33(0), m34(0),
			m41(0), m42(0), m43(0), m44(0) {
		}
	};

	struct alignas(16) dbl_matrix {
		union {
			matrix_elements elements;
			double m[4][4];
		};

		dbl_matrix() : elements() {}

		double& operator()(size_t row, size_t col) { return m[row][col]; }
		const double& operator()(size_t row, size_t col) const { return m[row][col]; }
	};

	struct alignas(16) fmatrix : public dbl_matrix {
		uemath::fplane x_plane;
		uemath::fplane y_plane;
		uemath::fplane z_plane;
		uemath::fplane w_plane;

		fmatrix() : dbl_matrix(), x_plane(), y_plane(), z_plane(), w_plane() {}
	};

	template< class t >
	class tarray
	{
	public:
		tarray() : data(), count(), max_count() {}
		tarray(t* data, uint32_t count, uint32_t max_count) :data(data), count(count), max_count(max_count) {}

		t get(uintptr_t idx)
		{
			return io->read< t >(reinterpret_cast<uintptr_t>(data) + (idx * sizeof(t)));
		}

		std::vector<t> get_all()
		{
			if (count <= 0 || count > max_count)
			{
				return {};
			}

			try
			{
				std::vector<t> buffer(count);

				io->read_physical(
					reinterpret_cast<PVOID>(data),
					buffer.data(),
					sizeof(t) * count
				);

				return buffer;
			}
			catch (const std::bad_alloc&)
			{
				return {};
			}
		}

		uintptr_t get_address()
		{
			return reinterpret_cast<uintptr_t>(data);
		}

		uint32_t get_count() const
		{
			return count;
		};

		uint32_t get_max_count() const
		{
			return max_count;
		};

		bool is_valid() const
		{
			return data != nullptr;
		};

		t& operator[](int i)
		{
			return this->data[i];
		};

		t* data;
		uint32_t count;
		uint32_t max_count;
	};

	class fname
	{
	public:
		int32_t ComparisonIndex;

		std::string to_string() const noexcept {
			return to_string(ComparisonIndex);
		}

		static std::string to_string(int32_t index) noexcept {
			static const uint64_t baseAddr = io->m_image_base + 0x17358300;

			int32_t DecryptedIndex = decrypt_index(index);
			uint64_t NamePoolChunk = io->read<uint64_t>(baseAddr + 8 * (HIWORD(DecryptedIndex) + 2)) + 2 * (uint16_t)DecryptedIndex;
			uint16_t Pool = io->read<uint16_t>(NamePoolChunk);

			if (((Pool ^ 0xD) & 0x3FF) == 0) {
				DecryptedIndex = decrypt_index(io->read<int32_t>(NamePoolChunk + 2));
				NamePoolChunk = io->read<uint64_t>(baseAddr + 8 * (HIWORD(DecryptedIndex) + 2)) + 2 * (uint16_t)DecryptedIndex;
				Pool = io->read<uint16_t>(NamePoolChunk);
			}

			int32_t Length = ((Pool ^ 0xD) & 0x3FF) * (((Pool & 0x8000u) != 0) ? 2 : 1);

			std::string NameBuffer;
			NameBuffer.resize(Length);

			io->read_physical(PVOID(NamePoolChunk + 2), &NameBuffer[0], Length);
			decrypt_fname(&NameBuffer[0], Length);
			return NameBuffer;
		}

		static int32_t decrypt_index(int32_t index) noexcept {
			if (!index) return 0;
			int32_t DecryptedIndex = ((index - 1) ^ 0xB97E0A91) + 1;
			return DecryptedIndex ? DecryptedIndex : 1182922095;
		}

		static void decrypt_fname(char* buffer, int length) noexcept {
			if (!length) return;

			int naan = 0x1358109 + 8252 * length;
			for (int i = 0; i < length; ++i) {
				buffer[i] ^= (naan - 12);
				naan = 0x1358109 + 8252 * naan;
			}
			buffer[length] = '\0';
		}
	};
}