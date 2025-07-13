/*static*/ inline InventoryItem::Item InventoryItem::getItemByHash( uint32_t itemHash )
{
	switch ( itemHash )
	{
	case 0x915AA574: return InventoryItem::SLING;
	case 0x04ADF9C7: return InventoryItem::TORCH;
	case 0xC024157C: return InventoryItem::PICKAXES;
	case 0x9608A818: return InventoryItem::TNT;
	case 0xEE6B156E: return InventoryItem::SHIELD;
	case 0xFB3D82AC: return InventoryItem::RAFT;
	case 0xCFC909C3: return InventoryItem::GAS_MASK;
	case 0xE51C8F72: return InventoryItem::CANTEEN;
	default: return InventoryItem::UNKNOWN;
	}
}

inline const char* InventoryItem::getName() const
{
	switch ( m_itemType )
	{
	case InventoryItem::SLING:    return "Sling";
	case InventoryItem::TORCH:    return "Torch";
	case InventoryItem::PICKAXES: return "Pickaxes";
	case InventoryItem::TNT:      return "TNT";
	case InventoryItem::SHIELD:   return "Shield";
	case InventoryItem::RAFT:     return "Raft";
	case InventoryItem::GAS_MASK: return "Gas Mask";
	case InventoryItem::CANTEEN:  return "Canteen";
	default: return "";
	}
}

inline uint32_t InventoryItem::getHash() const
{
	switch ( m_itemType )
	{
	case InventoryItem::SLING:    return 0x915AA574;
	case InventoryItem::TORCH:    return 0x04ADF9C7;
	case InventoryItem::PICKAXES: return 0xC024157C;
	case InventoryItem::TNT:      return 0x9608A818;
	case InventoryItem::SHIELD:   return 0xEE6B156E;
	case InventoryItem::RAFT:     return 0xFB3D82AC;
	case InventoryItem::GAS_MASK: return 0xCFC909C3;
	case InventoryItem::CANTEEN:  return 0xE51C8F72;
	default: return 0;
	}
}

inline uint32_t InventoryItem::getModelCRC() const
{
	switch ( m_itemType )
	{
	case InventoryItem::SLING:    return ItemModelsCRC::SLING;
	case InventoryItem::TORCH:    return ItemModelsCRC::TORCH;
	case InventoryItem::PICKAXES: return ItemModelsCRC::PICKAXES;
	case InventoryItem::TNT:      return ItemModelsCRC::TNT;
	case InventoryItem::SHIELD:   return ItemModelsCRC::SHIELD;
	case InventoryItem::RAFT:     return ItemModelsCRC::RAFT;
	case InventoryItem::GAS_MASK: return ItemModelsCRC::GAS_MASK;
	case InventoryItem::CANTEEN:  return ItemModelsCRC::CANTEEN;
	default: return 0;
	}
}

inline bool InventoryItem::isUnlocked() const
{
	return m_itemStruct->m_unlocked != 0;
}

inline void InventoryItem::setUnlocked( bool b )
{
	m_itemStruct->m_unlocked = b;
}
