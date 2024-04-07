#pragma once

#include <forward_list> // For the storage of observers.
#include <functional> // For the callbacks.
#include <memory> // For the smart pointers.

// NOTE: The following code **SHOULD BE** memory safe however I have not tested it extensively.

namespace voxels {
    template<typename... T>
    class Emitter;

    /**
     * @brief RAII observer class that manages the callback and the emitter.
     * 
     * @tparam T Argument types.
     */
    template <typename... T>
    class Observer {
    public:
        friend class Emitter<T...>;
        /// @brief Alias for the callback type.
        using Callback = std::function<void(T...)>;
        using Emitter_t = Emitter<T...>;

        Observer() = default;
        /**
         * @brief Construct a new observer and adds it to the emitter.
         * 
         * @param emitter The emitter to observe.
         * @param callback The callback.
         */
        Observer(std::weak_ptr<Emitter_t> emitter, Callback&& callback) noexcept
            : m_calllback(std::move(callback)), m_emitter(std::move(emitter)) 
        { 
            if(auto emitter = m_emitter.lock())
                emitter->addObserver(this);
        }

        ~Observer() {
            if(auto emitter = m_emitter.lock())
                emitter->removeObserver(this);
        }

    private:
        /**
         * @brief Invokes the callback.
         * 
         * @param arg The arguments.
         *
         * @note This is private so that only the friend class could call it.
         */
        inline void invoke(T... arg) const {
            if(m_calllback)
                m_calllback(arg...);
        }

        Callback m_calllback; //< The callback
        std::weak_ptr<Emitter_t> m_emitter; //< The emitter
    };

    /**
     * @brief Contains a list of observers that get invoked when an event is emitted.
     * 
     * @tparam T The argument types.
     */
    template<typename... T>
    class Emitter {
    public:
        friend class Observer<T...>;
        /// @brief Alias for the observer class.
        using Observer_t = Observer<T...>;

        Emitter() noexcept : m_observers() {  }

        Emitter(const Emitter&) = delete;
        Emitter& operator=(const Emitter&) = delete;
        Emitter(Emitter&&) = delete;
        Emitter& operator=(Emitter&&) = delete;

        /**
         * @brief Emits an event to all the observers.
         * 
         * @param arg Arguments to pass in.
         */
        inline void emit(T... arg) noexcept {
            for(auto observer : m_observers) {
                if(observer)
                    observer->invoke(arg...);
            }
        }

    private:
        /**
         * @brief Adds an observer pointer to the list.
         * 
         * @param observer The observer pointer to add.
         */
        inline void addObserver(Observer_t* observer) noexcept {
            m_observers.emplace_front(observer);
        }

        /**
         * @brief Removes an observer from the list.
         * 
         * @param observer The observer pointer to remove.
         */
        inline void removeObserver(Observer_t* observer) {
            m_observers.remove(observer);
        }

        std::forward_list<Observer_t*> m_observers = {}; //< Observer pointers.
    };
}