/*static*/ inline EIHarry* Gizmod::getHarry()
{
	// TODO: ASSUMES EIHarry!!! This is not the case in spirit temples!!
	return (EIHarry*) GLOBAL_STRUCT_PRIVATE::getInstance()->m_playerCharacter;
}

/*static*/ inline ERLevel* Gizmod::getLevel()
{
	ELevelController* ctrl = GLOBAL_STRUCT_PRIVATE::getInstance()->m_levelController;
	return ctrl ? ctrl->m_world : 0;
}

/*static*/ inline EUIHud* Gizmod::getHUD()
{
	return GLOBAL_STRUCT_PRIVATE::getInstance()->m_hud;
}

/*static*/ inline uint32_t Gizmod::getCurrentLevelCRC()
{
	return GLOBAL_STRUCT_PRIVATE::getInstance()->m_currentLevelCRC;
}
