/*static*/ inline EIHarry* Gizmod::getHarry()
{
	return *((EIHarry**) 0x917034);
}

/*static*/ inline ERLevel* Gizmod::getLevel()
{
	ERLevel** levelPtr = *((ERLevel***) 0x91702C);
	return levelPtr ? (*levelPtr) : 0;
}

/*static*/ inline EUIHud* Gizmod::getHUD()
{
	return *((EUIHud**) (0x917028 + 0x8C));
}

/*static*/ inline uint32_t Gizmod::getCurrentLevelCRC()
{
	return *((uint32_t*) 0x917088);
}
