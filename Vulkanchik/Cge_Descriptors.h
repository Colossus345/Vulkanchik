#pragma once

#include "CgeDevice.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace cge {

    class CgeDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(CgeDevice& cgeDevice) : cgeDevice{ cgeDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<CgeDescriptorSetLayout> build() const;

        private:
            CgeDevice& cgeDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        CgeDescriptorSetLayout(
            CgeDevice& CgeDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~CgeDescriptorSetLayout();
        CgeDescriptorSetLayout(const CgeDescriptorSetLayout&) = delete;
        CgeDescriptorSetLayout& operator=(const CgeDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        CgeDevice& cgeDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class CgeDescriptorWriter;
    };

    class CgeDescriptorPool {
    public:
        class Builder {
        public:
            Builder(CgeDevice& cgeDevice) : cgeDevice{ cgeDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<CgeDescriptorPool> build() const;

        private:
            CgeDevice& cgeDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        CgeDescriptorPool(
            CgeDevice& cgeDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~CgeDescriptorPool();
        CgeDescriptorPool(const CgeDescriptorPool&) = delete;
        CgeDescriptorPool& operator=(const CgeDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        CgeDevice& cgeDevice;
        VkDescriptorPool descriptorPool;

        friend class CgeDescriptorWriter;
    };
    class CgeDescriptorWriter {
    public:
        CgeDescriptorWriter(CgeDescriptorSetLayout& setLayout, CgeDescriptorPool& pool);

        CgeDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        CgeDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        CgeDescriptorSetLayout& setLayout;
        CgeDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}