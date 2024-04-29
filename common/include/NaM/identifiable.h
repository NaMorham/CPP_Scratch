#pragma once

#include <cstdint>
#include <string>

//-----------------------------------------------------------------------------

namespace NaM
{
    namespace CppScratch
    {
        typedef std::uint64_t TypeID_t; // TODO: uuids?

        //-----------------------------------------------------------------------------
        [[nodiscard]] inline const TypeID_t _GetId()
        {
            static TypeID_t lastId = 0;
            return ++lastId;
        }

        template <typename T>
        [[nodiscard]] inline const TypeID_t GetId() noexcept
        {
            static TypeID_t typeId = _GetId();
            return typeId;
        }

        //-----------------------------------------------------------------------------
        template<typename T>
        class Identifiable
        {
        private:
            static const TypeID_t ms_typeId;
            static TypeID_t ms_objId;
            static size_t ms_objCount;
            static std::string ms_typename;

            TypeID_t m_id;

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
            [[nodiscard]] inline const TypeID_t Id() const { return m_id; }

            [[nodiscard]] inline const TypeID_t TypeID() const { return ms_typeId; }
            [[nodiscard]] inline const TypeID_t TypeCount() const { return ms_objCount; }
            [[nodiscard]] inline const std::string& TypeName() const { return ms_typename; }

            [[nodiscard]] inline static const TypeID_t GetTypeID() { return ms_typeId; }
            [[nodiscard]] inline static const TypeID_t GetTypeCount() { return ms_objCount; }
            [[nodiscard]] inline static const std::string& GetTypeName() { return ms_typename; }
        };

        template<typename T>
        const TypeID_t Identifiable<T>::ms_typeId{GetId<T>()};

        template<typename T>
        TypeID_t Identifiable<T>::ms_objId{0};

        template<typename T>
        size_t Identifiable<T>::ms_objCount{0};

        template<typename T>
        std::string Identifiable<T>::ms_typename{""};

    };  // end namespace CppScratch

};  // end namespace NaM

//-----------------------------------------------------------------------------
