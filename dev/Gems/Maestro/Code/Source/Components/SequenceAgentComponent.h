/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Component/Component.h>
#include <Maestro/Bus/SequenceAgentComponentBus.h>
#include "SequenceAgent.h"

namespace AzFramework
{
    class TransformComponent;
}

namespace Maestro
{
    class SequenceAgentComponent
        : public AZ::Component
        , public SequenceAgentComponentRequestBus::MultiHandler
        , public SequenceAgent
    {
    public:
        friend class EditorSequenceAgentComponent;

        AZ_COMPONENT(SequenceAgentComponent, "{67DC06D3-1F16-4FAB-B3F8-D8C0A3AF4F61}");

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // SequenceAgentComponentRequestBus::Handler Interface
        void GetAnimatedPropertyValue(AnimatedValue& returnValue, const Maestro::SequenceComponentRequests::AnimatablePropertyAddress& animatableAddress) override;
        bool SetAnimatedPropertyValue(const Maestro::SequenceComponentRequests::AnimatablePropertyAddress& animatableAddress, const AnimatedValue& value) override;

        AZ::Uuid GetAnimatedAddressTypeId(const AnimatablePropertyAddress& animatableAddress) override;

        void ConnectSequence(const AZ::EntityId& sequenceEntityId) override;
        void DisconnectSequence() override;
        //~SequenceAgentComponentRequestBus::Handler Interface
        //////////////////////////////////////////////////////////////////////////
        
    protected:
        // Required Reflect function.
        static void Reflect(AZ::ReflectContext* context);

        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
        {
            dependent.push_back(AZ_CRC("TransformService", 0x8ee22c50));
        }

        // Override from SequenceAgent
        const AZ::Uuid& GetComponentTypeUuid(const AZ::Component& component) const override
        {
            return component.RTTI_GetType();
        }

    private:
        // connect and disconnect to all SequenceComponents registered with us
        void ConnectAllSequences();
        void DisconnectAllSequences();

        // set of ids of all unique Entities with SequenceComponent instances connected to this Agent
        AZStd::unordered_set<AZ::EntityId>       m_sequenceEntityIds;
    };

} // namespace Maestro