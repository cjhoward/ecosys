/*
 * Copyright (C) 2015  Christopher J. Howard
 *
 * This file is part of Ecosys.
 *
 * Ecosys is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ecosys is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ecosys.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ecosys/component-manager.hpp>
#include <ecosys/component-observer.hpp>

namespace ecs
{

void ComponentManager::addComponentObserver(ComponentObserver* observer)
{
	componentObservers.push_back(observer);
}

void ComponentManager::removeComponentObserver(ComponentObserver* observer)
{
	componentObservers.remove(observer);
}

void ComponentManager::addComponent(EntityID entity, Component* component)
{
	ComponentType componentType = component->getComponentType();
	
	entityMap[entity][componentType] = component;
	
	// Notify observers
	for (auto observer: componentObservers)
	{
		observer->componentAdded(entity, component);
	}
}

Component* ComponentManager::removeComponent(EntityID entity, ComponentType type)
{
	ComponentMap& componentMap = entityMap[entity];
	
	auto it = componentMap.find(type);
	if (it == componentMap.end())
	{
		return nullptr;
	}

	Component* component = it->second;

	// Notify observers
	for (auto observer: componentObservers)
	{
		observer->componentRemoved(entity, component);
	}

	componentMap.erase(it);
	
	return component;
}

Component* ComponentManager::getComponent(EntityID entity, ComponentType type)
{
	ComponentMap& componentMap = entityMap[entity];
	
	auto it = componentMap.find(type);
	if (it == componentMap.end())
	{
		return nullptr;
	}
	
	return it->second;
}

} // namespace ecs
