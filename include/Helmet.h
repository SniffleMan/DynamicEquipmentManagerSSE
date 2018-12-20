#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate

#include "ISerializableForm.h"  // ISerializableForm
#include "json.hpp"  // json
#include "PlayerInventoryChanges.h"  // InventoryChangesVisitor

#include "RE/BSAnimationGraphEvent.h"  // BSAnimationGraphEvent
#include "RE/BSTEvent.h"  // BSTEventSink, EventResult, BSTEventSource
#include "RE/FormTypes.h"  // TESObjectARMO, EnchantmentItem
#include "RE/Memory.h"  // TES_HEAP_REDEFINE_NEW
#include "RE/TESEquipEvent.h"  // TESEquipEvent


namespace Helmet
{
	class Helmet : public ISerializableForm
	{
	public:
		Helmet();
		virtual ~Helmet();

		virtual const char*		ClassName() const override;
		void					Clear();
		bool					Save(json& a_save);
		bool					Load(json& a_load);
		void					SetEnchantmentForm(UInt32 a_formID);
		UInt32					GetLoadedEnchantmentFormID();
		RE::TESObjectARMO*		GetArmorForm();
		RE::EnchantmentItem*	GetEnchantmentForm();

	protected:
		enum
		{
			kVersion = 1
		};


		class Enchantment : public ISerializableForm
		{
		public:
			Enchantment();
			virtual ~Enchantment();

			virtual const char*		ClassName() const override;
			RE::EnchantmentItem*	GetEnchantmentForm();

		protected:
			enum
			{
				kVersion = 1
			};
		};


		Enchantment _enchantment;
	};


	class HelmetTaskDelegate : public TaskDelegate
	{
	public:
		class HelmetEquipVisitor : public InventoryChangesVisitor
		{
		public:
			virtual bool Accept(RE::InventoryEntryData* a_entry) override;
		};


		class HelmetUnEquipVisitor : public InventoryChangesVisitor
		{
		public:
			virtual bool Accept(RE::InventoryEntryData* a_entry) override;
		};


		constexpr HelmetTaskDelegate(bool a_equip) :
			_equip(a_equip)
		{}

		virtual void Run() override;
		virtual void Dispose() override;

		TES_HEAP_REDEFINE_NEW();

	private:
		bool _equip;
	};


	class DelayedHelmetLocator : public TaskDelegate
	{
	public:
		class Visitor : public InventoryChangesVisitor
		{
		public:
			constexpr explicit Visitor(UInt32 a_formID) :
				_formID(a_formID)
			{}

			virtual bool Accept(RE::InventoryEntryData* a_entry) override;

		private:
			UInt32 _formID;
		};


		constexpr explicit DelayedHelmetLocator(UInt32 a_formID) :
			_formID(a_formID)
		{}

		virtual void Run() override;
		virtual void Dispose() override;

		TES_HEAP_REDEFINE_NEW();

	private:
		UInt32 _formID;
	};


	class TESEquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource) override;
	};


	class BSAnimationGraphEventHandler : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
	{
	public:
		virtual RE::EventResult ReceiveEvent(RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;
	};


	extern Helmet g_lastEquippedHelmet;
	extern TESEquipEventHandler g_equipEventSink;
	extern BSAnimationGraphEventHandler g_animationGraphEventSink;
}