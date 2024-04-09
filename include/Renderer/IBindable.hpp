#pragma once

namespace renderer {
    /**
     * @brief Interface for a bindable object.
     */
    class IBindable {
    public:
        virtual ~IBindable() = default;
        /**
         * @brief Binds this object as the current one.
         */
        virtual void bind() const noexcept = 0;
        /**
         * @brief Ubinds the current object and leaves it unbound.
         */
        virtual void unBind() const noexcept = 0;
    };
}