
#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <optional>
#include <memory>
#include <map>
#include "package.hpp"
#include "storage_types.hpp"
#include <functional>
#include "helpers.hpp"

enum class ReceiverType{
    WORKER, STOREHOUSE
};

class IPackageReceiver
        {
        public:
            using iterator =IPackageStockpile::const_iterator;
            virtual void receive_package(Package&& p) = 0;
            virtual ElementID get_id() const = 0;

            virtual iterator begin() const = 0;
            virtual iterator end() const = 0;
            virtual iterator cbegin() const = 0;
            virtual iterator cend() const = 0;
            virtual ReceiverType get_receiver_type() const = 0;
        };

class ReceiverPreferences
        {
        private:
            void rebuild_pref();
            ProbabilityGenerator mRng;

        public:
            using preferences_t = std::map<IPackageReceiver*, double>;
            using const_iterator = preferences_t::const_iterator;
            using iterator = preferences_t::iterator;
            ReceiverPreferences(ProbabilityGenerator probability_function = probability_generator): mRng(std::move(probability_function)) {};
            preferences_t preferences_;
            void add_receiver(IPackageReceiver* r);
            void remove_receiver(IPackageReceiver* r);
            double get_probability(IPackageReceiver *key) {return preferences_[key];}
            IPackageReceiver* choose_receiver();
            const preferences_t& get_preferences() const { return preferences_; }
            iterator begin()              { return preferences_.begin();  }
            iterator end()                { return preferences_.end();    }
            const_iterator cbegin() const { return preferences_.cbegin(); }
            const_iterator cend()   const { return preferences_.cend();   }
        };

class PackageSender
        {
        private:
            std::optional<Package> mBuffer;
        public:
            ReceiverPreferences receiver_preferences_;
            PackageSender(PackageSender&& ) = default;
            explicit PackageSender() : mBuffer() , receiver_preferences_() {};
            void send_package();
            std::optional<Package>& get_sending_buffer() {return mBuffer;};
        protected:
            void push_package(Package&& pck);
        };

class Ramp : public PackageSender
        {
        private:
            TimeOffset mOffset;
            ElementID mID;
        public:
            Ramp(ElementID id, TimeOffset di) :PackageSender(), mOffset(di), mID(id) {};
            void deliver_goods(Time t);
            TimeOffset get_delivery_interval()  const { return mOffset; }
            ElementID get_id()                  const { return mID;     }
        };

class Worker : public PackageSender, public IPackageReceiver
        {
        private:
            Time mTime = 0;
            TimeOffset mOffset;
            ElementID mID;
            std::optional<Package> mWorkerBuffer;
            std::unique_ptr<IPackageQueue> mUniquePtr;
        public:
            Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue_ptr);
            void do_work(Time t);
            TimeOffset get_processing_duration()     const { return mOffset; }
            Time get_package_processing_start_time() const { return mTime;   }
            ElementID get_ID_from_buffer() const          { return mWorkerBuffer->get_id();}
            ElementID get_id()const override { return mID; }
            void receive_package(Package&& pck) override;
            ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }
            PackageQueueType get_queue_type() const {return mUniquePtr->get_queue_type();}
            iterator begin() const override { return  mUniquePtr->begin(); }
            iterator end() const override   { return  mUniquePtr->end()  ; }
            iterator cbegin()const override { return  mUniquePtr->cbegin() ; }
            iterator cend() const override  { return  mUniquePtr->cend() ; }

        };

class Storehouse : public IPackageReceiver
        {
        private:
            std::unique_ptr<IPackageStockpile> mUniquePtr;
            ElementID mID;

        public:
            Storehouse(ElementID id) : mID(id) {}
            Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
            ElementID get_id() const override { return mID; }
            void receive_package(Package&& pck) override;
            ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }
            iterator begin() const override { return mUniquePtr->begin() ; }
            iterator end() const override   { return mUniquePtr->end()   ; }
            iterator cbegin()const override { return mUniquePtr->cbegin(); }
            iterator cend() const override  { return mUniquePtr->cend()  ; }
        };


#endif //NETSIM_NODES_HPP
