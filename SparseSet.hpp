#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

constexpr uint32_t INVALID_HANDLE = std::numeric_limits<uint32_t>::max();

template<typename T>
class SparseSet
{
  private:
    
    std::vector<T>        dense{};
    std::vector<uint32_t> associated_handles{}; // for any index 'i' in dense_, associated_handles_ stores the handle
    std::vector<uint32_t> sparse{};             // associated to the object at associated_handles_[i].
    
  public:
    SparseSet(size_t size = 1000)
    {
        sparse.resize(size, INVALID_HANDLE);
    }

    void add(uint32_t handle, const T& object)
    {
        if (handle < sparse.size())
        {
            if (sparse[handle] == INVALID_HANDLE)
            {
                uint32_t index = dense.size();
                dense.push_back(object);
                associated_handles.push_back(handle);
                sparse[handle] = index;
            }
            else
            {
                throw std::runtime_error("SparseSet::Add() failed. Handle already exists.");
            }
        }
        else
        {
            throw std::runtime_error("SparseSet::Add() failed. Handle is out of range.");
        }
    }

    T& get(uint32_t handle)
    {
        size_t index = 0;
        if (handle < sparse.size())
        {
            index = sparse[handle];
        }
        else
        {
            throw std::runtime_error("SparseSet::Get() failed. Handle is out of range.");
        }

        if (index != INVALID_HANDLE)
        {
            return dense[index];
        }
        else
        {
            std::string message = std::string("SparseSet::Get() failed. Nothing exists at this handle -> ")
                                + std::to_string(handle);
            throw std::runtime_error(message);
        }
    }

    const T& get_const(uint32_t handle) const
    {
        size_t index = 0;
        if (handle < sparse.size())
        {
            index = sparse[handle];
        }
        else
        {
            throw std::runtime_error("SparseSet::GetConst() failed. Handle is out of range.");
        }

        if (index != INVALID_HANDLE)
        {
            return dense[index];
        }
        else
        {
            std::string message = std::string("SparseSet::GetConst() failed. Nothing exists at this handle -> ")
                                + std::to_string(handle);
            throw std::runtime_error(message);
        }
    }

    void remove(uint32_t handle)
    {
        if (handle < sparse.size())
        {
            if (sparse[handle] != INVALID_HANDLE)
            {
                size_t index_to_delete = sparse[handle];
                T last_object = dense.back();
                uint32_t last_associated_handle = associated_handles.back();

                dense[index_to_delete] = last_object;
                associated_handles[index_to_delete] = last_associated_handle;
                sparse[last_associated_handle] = index_to_delete;

                sparse[handle] = INVALID_HANDLE;

                associated_handles.pop_back();
                dense.pop_back();
            }
            else
            {
                throw std::runtime_error("SparseSet::Delete() failed. Nothing exists at this handle.");
            }

        }
        else
        {
            throw std::runtime_error("SparseSet::Delete() failed. Handle is out of range.");
        }

    }

    uint32_t get_associated_handle(size_t dense_index)
    {
        if (dense_index < dense.size() and dense.size() == associated_handles.size())
        {   
            return associated_handles[dense_index];
        }
        else
        {
            std::string dense_size = std::to_string(static_cast<size_t>(dense.size()));
            std::string ah_size = std::to_string(static_cast<size_t>(associated_handles.size()));
            std::string dis = std::to_string(dense_index);
            std::string message = std::string("SparseSet::GetAssociatedHandle() failed. Dense index may be out of range.")
                                + "\nDense index: " + dis
                                + "\nSize of associated_handles_: " + ah_size
                                + "\nSize of dense_: " + dense_size;
            throw std::runtime_error(message);
        }
    }

    std::vector<T>& get_dense()
    {
        return dense;
    }

    bool has(uint32_t handle)
    {
        return (handle < sparse.size() and sparse[handle] != INVALID_HANDLE);
    }
};
