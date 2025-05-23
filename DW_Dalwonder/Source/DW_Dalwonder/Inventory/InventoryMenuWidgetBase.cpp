// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryMenuWidgetBase.h"
#include "Inventory/InventoryWidgetBase.h"

UInventoryWidgetBase* UInventoryMenuWidgetBase::GetInventoryWidget() const
{
    if (InventoryWidget)
    {
        return InventoryWidget;
    }
    return nullptr;
}
