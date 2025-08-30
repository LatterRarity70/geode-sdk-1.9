// backport from Geode's NodeIDs

#include <Geode/Geode.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/NodeIDs.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(LevelInfoLayer) {
	auto winSize = CCDirector::get()->getWinSize();

	setIDSafe<CCSprite>(this, 0, "background");

	size_t iconOffset = 0;

	setIDSafe<CCSprite>(this, 1, "bottom-left-art");
	setIDSafe<CCSprite>(this, 2, "bottom-right-art");

	if (m_level->m_originalLevel != 0) {
		setIDSafe<CCSprite>(this, 3, "copy-indicator");
		iconOffset++;
	}

	setIDSafe<CCSprite>(this, 3 + iconOffset, "length-icon");
	setIDSafe<CCSprite>(this, 4 + iconOffset, "downloads-icon");
	setIDSafe<CCSprite>(this, 5 + iconOffset, "likes-icon");

	size_t labelOffset = 0;

	setIDSafe<CCLabelBMFont>(this, labelOffset++, "title-label");
	setIDSafe<CCLabelBMFont>(this, labelOffset++, "creator-label");

	setIDSafe<CCLabelBMFont>(this, labelOffset++, "downloads-label");
	setIDSafe<CCLabelBMFont>(this, labelOffset++, "length-label");
	setIDSafe<CCLabelBMFont>(this, labelOffset++, "likes-label");
	setIDSafe<CCLabelBMFont>(this, labelOffset++, "stars-label");

	setIDSafe<CCSprite>(this, 8 + iconOffset, "stars-icon");

	if (m_difficultyIcon)
		m_difficultyIcon->setID("difficulty-sprite");
	if (m_loadingCircle)
		m_loadingCircle->setID("loading-circle");
	if (m_featuredCoin)
		m_featuredCoin->setID("featured-sprite");

	setIDSafe<CustomSongWidget>(this, 0, "custom-songs-widget");

	if (auto menu = this->getChildByType<CCMenu>(0)) {
		menu->setID("play-menu");
		setIDSafe(menu, 0, "play-button");
	}

	if (auto menu = this->getChildByType<CCMenu>(2)) {
		menu->setID("back-menu");
		auto backBtn = setIDSafe(menu, 0, "back-button");
		menu->setPositionX(
			menu->getPositionX() + 100.f / 2 - getSizeSafe(backBtn).width / 2
		);
		menu->setContentSize({100.f, 50.f});
		menu->setLayout(
			RowLayout::create()
				->setAxisAlignment(AxisAlignment::Start)
		);
	}

	if (auto menu = this->getChildByType<CCMenu>(1)) {
		menu->setID("right-side-menu");

		auto leftSideMenu = CCMenu::create();
		leftSideMenu->setPosition(30.f, winSize.height / 2);
		leftSideMenu->setLayout(ColumnLayout::create());
		leftSideMenu->setID("left-side-menu");
		leftSideMenu->setContentSize({50.f, 145.f});
		this->addChild(leftSideMenu);

		menu->setPosition(winSize.width - 30.f, winSize.height / 2);

		for (auto child : CCArrayExt<CCNode*>(menu->getChildren())) {
			if (child->getPositionX() < 0.f) {
				child->retain();
				child->removeFromParent();
				leftSideMenu->addChild(child);
				child->release();
			}
			child->setPositionX(0.f);
		}

		setIDSafe(menu, 0, "delete-button");
		setIDSafe(menu, 1, "refresh-button");
		setIDSafe(menu, 2, "info-button");

		if (m_level->m_stars == 0) {
			setIDSafe(menu, 3, "difficulty-button");
			setIDSafe(menu, 4, "like-button");
			setIDSafe(menu, 5, "rate-button");
		} else {
			setIDSafe(menu, 3, "like-button");
		}

		menu->setPosition(
			menu->getPositionX() + static_cast<CCNode*>(menu->getChildren()->objectAtIndex(0))->getPositionX(),
			winSize.height / 2
		);
		menu->setContentSize({60.f, winSize.height - 15.f});
		menu->setLayout(
			ColumnLayout::create()
				->setGap(3.f)
				->setAxisAlignment(AxisAlignment::End)
				->setAxisReverse(true)
		);

		if (auto deleteServerBtn = getChildBySpriteFrameName(menu, "GJ_deleteServerBtn_001.png")) {
			deleteServerBtn->setID("delete-server-button");
		}

		if (auto copyBtn = getChildBySpriteFrameName(leftSideMenu, "GJ_duplicateBtn_001.png")) {
			copyBtn->setID("copy-button");
		}

		if (auto copyBtn = getChildBySpriteFrameName(leftSideMenu, "GJ_duplicateLockedBtn_001.png")) {
			copyBtn->setID("copy-button");
		}

		if (auto btn = leftSideMenu->getChildByID("delete-button"))
			btn->setZOrder(-1);
		if (auto btn = leftSideMenu->getChildByID("copy-button"))
			btn->setZOrder(-2);

		menu->updateLayout();
		leftSideMenu->updateLayout();
	}

	if (auto menu = this->getChildByType<CCMenu>(3)) {
		menu->setID("bottom-right-menu");

		auto statsBtn = setIDSafe(menu, 0, "stats-button");
		menu->setPositionY(
			menu->getPositionY() + 100.f / 2 - getSizeSafe(statsBtn).height / 2
		);
		menu->setContentSize({50.f, 100.f});
		menu->setLayout(
			ColumnLayout::create()
				->setAxisAlignment(AxisAlignment::Start)
		);
	}

	/**
	 * Verify node IDs
	 */
	if (m_lengthLabel != getChildByID("length-label")) {
		log::warn("Node IDs are incorrect: m_lengthLabel != getChildByID(\"length-label\")");
	}

	if (m_downloadsLabel != getChildByID("downloads-label")) {
		log::warn("Node IDs are incorrect: m_downloadsLabel != getChildByID(\"downloads-label\")");
	}

	if (m_likesLabel != getChildByID("likes-label")) {
		log::warn("Node IDs are incorrect: m_likesLabel != getChildByID(\"likes-label\")");
	}

	if (m_starIcon != getChildByID("stars-icon")) {
		log::warn("Node IDs are incorrect: m_starsIcon != getChildByID(\"stars-icon\")");
	}

	if (m_starsLabel != getChildByID("stars-label")) {
		log::warn("Node IDs are incorrect: m_starsLabel != getChildByID(\"stars-label\")");
	}
}

struct LevelInfoLayerIDs : Modify<LevelInfoLayerIDs, LevelInfoLayer> {
	static void onModify(auto& self) {
		if (!self.setHookPriority("LevelInfoLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) {
			log::warn("Failed to set LevelInfoLayer::init hook priority, node IDs may not work properly");
		}
	}

	bool init(GJGameLevel* level) {
		if (!LevelInfoLayer::init(level))
			return false;

		NodeIDs::get()->provide(this);

		return true;
	}

	void setupProgressBars() {
		LevelInfoLayer::setupProgressBars();

		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 1))->setID("practice-mode-label");
		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 2))->setID("normal-mode-label");
		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 3))->setID("practice-mode-percentage");
		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 4))->setID("normal-mode-percentage");
		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 5))->setID("practice-mode-bar");
		static_cast<CCNode*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 6))->setID("normal-mode-bar");
	}
};
