#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(std::string FilePath)
{
	//Load(FilePath);
}

bool SpriteSheet::RequestAnimation(std::string Name)
{
	auto anim = GetAnimation(Name);
	if (anim == nullptr) return false;

	m_CurrentAnimation = anim;
	m_CurrentAnimation->UpdateFrame(true);

	return true;
}

void SpriteSheet::Update()
{
	if (m_CurrentAnimation == nullptr) return;
	m_CurrentAnimation->UpdateFrame();
}

Animation * SpriteSheet::GetAnimation(std::string Name)
{
	for (auto x : m_Animations)
		if (Name == x.GetName())
			return &x;

	return nullptr;
}

//TODO: Unify naming, pN(XXXX) Pointer+Node but considering they're all local, makes it hard to read (pick one)

bool SpriteSheet::Save()
{
	/*
	XFactor
	YFactor
	Animations
	Name
	Repeats //TODO: maybe don't store this?
	StartRow
	StartCol
	Frames
	*/

	using namespace tinyxml2;

	XMLDocument xmlDoc;
	auto *pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertFirstChild(pRoot);

	//XFactor, YFactor
	auto *pNXFactor = xmlDoc.NewElement("XFactor");
	pNXFactor->SetText(m_XFactor);
	pRoot->InsertEndChild(pNXFactor);
	auto *pNYFactor = xmlDoc.NewElement("YFactor");
	pNYFactor->SetText(m_YFactor);
	pRoot->InsertEndChild(pNYFactor);

	//Animations
	auto *pNAnimations = xmlDoc.NewElement("Animations");
	pRoot->InsertEndChild(pNAnimations);
	for (auto x : m_Animations)
		x.Save(&xmlDoc, pNAnimations);

	XMLError eResult = xmlDoc.SaveFile(GetXMLFilePath().c_str());
	return eResult == 0;
}


//SpriteSheets are split into a number of rectangles of a set size, x by y
//Animations are given the x y of their spritesheet so they can calculate their bidness

Animation::Animation(std::string Name, bool Repeats, int StartRow, int StartCol, int FrameCount) :
	m_Name(Name),
	m_Repeats(Repeats),
	m_StartRow(StartRow),
	m_StartCol(StartCol),
	m_FrameCount(FrameCount)
{

}

//TODO: Simplify block
Vector2 Animation::UpdateFrame(bool Start)
{
	if (m_CurrentFrame == m_FrameCount - 1)
	{
		if (m_Repeats)
			m_CurrentFrame = 0;
	}
	else
		m_CurrentFrame++;

	if (Start)
		m_CurrentFrame = 0;

	return Vector2(m_StartRow, m_StartCol + m_CurrentFrame);
}

bool Animation::IsAnimationFinished()
{
	return !m_Repeats && m_CurrentFrame == m_FrameCount - 1;
}

void Animation::Save(tinyxml2::XMLDocument *Doc, tinyxml2::XMLNode * RootNode)
{
	if (Doc == nullptr || RootNode == nullptr) return;

	auto *n = Doc->NewElement("Name");
	n->SetText(m_Name.c_str());
	RootNode->InsertEndChild(n);

	auto *r = Doc->NewElement("Repeats");
	r->SetText(r);
	RootNode->InsertEndChild(r);

	auto *sr = Doc->NewElement("StartRow");
	sr->SetText(sr);
	RootNode->InsertEndChild(sr);

	auto *sc = Doc->NewElement("StartCol");
	sc->SetText(sc);
	RootNode->InsertEndChild(sc);

	auto *fc = Doc->NewElement("FrameCount");
	fc->SetText(fc);
	RootNode->InsertEndChild(fc);
}