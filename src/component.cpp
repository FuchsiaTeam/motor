#include "motor/interfaces/component.hpp"
#include "motor/object/componentholder.hpp"

namespace Motor {

    void IComponent::callComponentEvent(IComponentEvent* eventData, bool _delete) {
        getOwner()->onComponentEvent(eventData);
        if(_delete) delete eventData;
    }
}