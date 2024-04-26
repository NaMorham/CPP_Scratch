#pragma once

#include <cstdint>
#include <string>

//-----------------------------------------------------------------------------

namespace NaM
{
    namespace CppScratch
    {
        typedef std::uint64_t TypeID; // TODO: uuids?

        //-----------------------------------------------------------------------------
        [[nodiscard]] inline const TypeID _GetId()
        {
            static TypeID lastId = 0;
            return ++lastId;
        }

        template <typename T>
        [[nodiscard]] inline const TypeID GetId() noexcept
        {
            static TypeID typeId = _GetId();
            return typeId;
        }

        //-----------------------------------------------------------------------------
        template<typename T>
        class Identifiable
        {
        private:
            static const TypeID ms_typeId;
            static TypeID ms_objId;
            static size_t ms_objCount;
            static std::string ms_typename;

            TypeID m_id;

        protected:
            Identifiable() = delete;
            Identifiable(const std::string& name) : m_id(++ms_objId)
            {
                ++ms_objCount;
                ms_typename.assign(name);
            }

            Identifiable(const Identifiable& orig) : m_id(++ms_objId)  // new ID when copying
            {
                ++ms_objCount;
            }

            virtual ~Identifiable()
            {
                m_id = 0;
                --ms_objCount;
            }

        public:
            [[nodiscard]] inline const TypeID GetID() const { return m_id; }
            [[nodiscard]] inline const TypeID GetTypeID() const { return ms_typeId; }
            [[nodiscard]] inline const TypeID GetTypeCount() const { return ms_objCount; }
            [[nodiscard]] inline const std::string& GetTypeName() const { return ms_typename; }
        };

        template<typename T>
        const TypeID Identifiable<T>::ms_typeId{GetId<T>()};

        template<typename T>
        TypeID Identifiable<T>::ms_objId{0};

        template<typename T>
        TypeID Identifiable<T>::ms_objCount{0};

        template<typename T>
        std::string Identifiable<T>::ms_typename{""};

    };  // end namespace CppScratch

};  // end namespace NaM

//-----------------------------------------------------------------------------
