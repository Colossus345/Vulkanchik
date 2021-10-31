#pragma once

#include "CgeModel.h"

#include<memory>

namespace cge {

	struct Transform2dComponent {

		glm::vec2 translation{};
		glm::vec2 scale{1.f,1.f};
		float rotation;


		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMat = { {c,s},{-s,c} };
			glm::mat2 scaleMat{ {scale.x,.0f},{.0f,scale.y} };
			return rotMat * scaleMat;
		}
	};

	class CgeGameObject
	{
	public:


		using id_t = unsigned int;


		static CgeGameObject createGameObject() {
			static id_t  currentId = 0;
			return CgeGameObject{ currentId++ };
		}

		CgeGameObject(const CgeGameObject&) = delete;
		CgeGameObject &operator=(const CgeGameObject &) = delete;
		CgeGameObject(CgeGameObject&&) = default;
		CgeGameObject& operator=(const CgeGameObject&&) = delete;

		
		id_t get_id(){ return id; }

		std::shared_ptr<CgeModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d{};

	private:
		CgeGameObject(id_t objId) :id{ objId } {};


		id_t id;

	};

}