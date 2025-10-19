#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <format>

constexpr uint32_t INVALID_HANDLE = std::numeric_limits<uint32_t>::max();

template<typename T>
class SparseSet {
private:
    std::vector<T>        dense{};
    std::vector<uint32_t> associated_handles{}; // for any index 'i' in dense_, associated_handles_ stores the handle
    std::vector<uint32_t> sparse{};             // associated to the object at associated_handles_[i].

    uint32_t              next_handle = 0;
    std::vector<uint32_t> free_handles{};       // stores retired entity ids that can be reused
public:
    SparseSet(size_t size = 1000) {
        sparse.resize(size, INVALID_HANDLE);
    }

    uint32_t add(const T& object) {
        uint32_t handle;
        if (free_handles.empty()) {
            handle = next_handle;
            next_handle++;
        } else {
            handle = free_handles.back();
            free_handles.pop_back();
        }
    #ifdef _DEBUG
        if (sparse[handle] != INVALID_HANDLE) {
            throw std::runtime_error("SparseSet::Add() failed. Handle already exists.");
        }
    #endif 
        uint32_t index = dense.size();
        dense.push_back(object);
        associated_handles.push_back(handle);
        sparse[handle] = index;
        return handle;
    }

    T& get(uint32_t handle) {
        size_t index = 0;
    #ifdef _DEBUG
        if (handle > sparse.size()) { throw std::runtime_error("SparseSet::Get() failed. Handle is out of range."); }
        if (sparse[handle] == INVALID_HANDLE) {
            std::string msg = std::format("SparseSet::Get() failed. Nothing exists at this handle -> {}", handle);
            throw std::runtime_error(msg);
        }
    #endif
        index = sparse[handle];
        return dense[index];
    }

    void remove(uint32_t handle) {
    #ifdef _DEBUG
        if (handle > sparse.size()) {
            throw std::runtime_error("SparseSet::Delete() failed. Handle is out of range.");
        }
        if (sparse[handle] == INVALID_HANDLE) {
            throw std::runtime_error("SparseSet::Delete() failed. Nothing exists at this handle.");
        }
    #endif 
        size_t index_to_delete = sparse[handle];
        T last_object = dense.back();
        uint32_t last_associated_handle = associated_handles.back();

        dense[index_to_delete] = last_object;
        associated_handles[index_to_delete] = last_associated_handle;
        sparse[last_associated_handle] = index_to_delete;

        sparse[handle] = INVALID_HANDLE;

        associated_handles.pop_back();
        dense.pop_back();
        free_handles.push_back(handle);
    }

    uint32_t get_associated_handle(size_t dense_index) {
    #ifdef _DEBUG
        if (dense_index > dense.size() or dense.size() != associated_handles.size()) {
            std::string message = std::string("SparseSet::GetAssociatedHandle() failed. Dense index may be out of range.");
            throw std::runtime_error(message);
        }
    #endif
        return associated_handles[dense_index];
    }

    std::vector<T>& get_dense() { return dense; }

    bool has(uint32_t handle) {
        return (handle < sparse.size() and sparse[handle] != INVALID_HANDLE);
    }
};