#pragma once

#include "CgeModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include<memory>
#include <unordered_map>

namespace cge {

	struct TransformComponent {

		glm::vec3 translation{};
		glm::vec3 scale{ 1.f,1.f,1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	

	class CgeGameObject
	{
	public:


		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, CgeGameObject>;

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
		TransformComponent transform{};

	private:
		CgeGameObject(id_t objId) :id{ objId } {};


		id_t id;

	};

}