#include <Geode/Bindings.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/NodeIDs.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(EndLevelLayer) {
		m_mainLayer->setID("main-layer");

		int idx = 0;
		setIDs(
				m_mainLayer,
				idx,
				"background",
				"back-menu",
				"chain-left",
				"chain-right"
		);
		idx += 4;

		if (auto backMenu = m_mainLayer->getChildByID("back-menu")) {
			auto backBtn = setIDSafe(backMenu, 0, "back-button");
			backMenu->setPositionX(
				backMenu->getPositionX() + 100.f / 2 - getSizeSafe(backBtn).width / 2
			);
			backMenu->setContentSize({ 100.f, 50.f });
			backMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start));
		}

		if (auto levelTxt = getChildBySpriteFrameName(m_mainLayer, "GJ_levelComplete_001.png")) {
				levelTxt->setID("level-complete-text");
				idx += 1;
		}
		else if (auto practiceTxt = getChildBySpriteFrameName(m_mainLayer, "GJ_practiceComplete_001.png")) {
				practiceTxt->setID("practice-complete-text");
				idx += 1;
		}

		for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
				if (auto bmFont = typeinfo_cast<CCLabelBMFont*>(child)) {
						std::string_view view = bmFont->getString();

						if (view.starts_with("Attempts")) {
								bmFont->setID("attempts-label");
								idx += 1;
						}
						else if (view.starts_with("Jumps")) {
								bmFont->setID("jumps-label");
								idx += 1;
						}
						else if (view.starts_with("Time")) {
								bmFont->setID("time-label");
								idx += 1;
						}
				}
		}

		if (auto endText = typeinfo_cast<CCLabelBMFont*>(m_mainLayer->getChildren()->objectAtIndex(idx))) {
				endText->setID("end-text");
				idx += 1;
		}

		if (auto textArea = typeinfo_cast<TextArea*>(m_mainLayer->getChildren()->objectAtIndex(idx))) {
				textArea->setID("complete-message");
				idx += 1;
		}

		setIDs(
				m_mainLayer,
				idx,
				"button-menu"
		);
		idx += 1;

		auto menu = static_cast<CCMenu*>(m_mainLayer->getChildByID("button-menu"));

		setIDs(
				menu,
				0,
				"retry-button",
				"exit-button"
		);

		if (auto editButton = getChildBySpriteFrameName(menu, "GJ_editBtn_001.png")) {
				editButton->setID("edit-button");
		}

		int currentCoin = 1;
		std::vector<CCPoint> coinPos;
		for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
				for (auto framename : {
						"secretCoin_b_01_001.png",
						"secretCoin_2_b_01_001.png"
				}) {
						if (isSpriteFrameName(child, framename)) {
								child->setID(fmt::format("coin-{}-background", currentCoin));
								coinPos.push_back(child->getPosition());
								currentCoin += 1;
								idx += 1;
						}
				}
		}

		for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
				for (int i = 1; i < currentCoin; i++) {
						if (child->getID().empty() && child->getPosition() == coinPos[i - 1]) {
								child->setID(fmt::format("coin-{}-sprite", i));
						}
				}
		}

		for (auto child : CCArrayExt<CCNode*>(m_coinEffects)) {
				for (int i = 1; i < currentCoin; i++) {
						if (child->getID().empty() && child->getPosition() == coinPos[i - 1]) {
								child->setID(fmt::format("coin-{}-sprite", i));
						}
				}
		}

		if (PlatformToolbox::isControllerConnected()) {
				setIDs(
						m_mainLayer,
						idx,
						"controller-retry-hint",
						"controller-exit-hint"
				);
				idx += 2;
		}
}

struct EndLevelLayerIDs : Modify<EndLevelLayerIDs, EndLevelLayer> {
		static void onModify(auto& self) {
				if (!self.setHookPriority("EndLevelLayer::customSetup", GEODE_ID_PRIORITY)) {
						log::warn("Failed to set EndLevelLayer::customSetup hook priority, node IDs may not work properly");
				}
				if (!self.setHookPriority("EndLevelLayer::showLayer", GEODE_ID_PRIORITY)) {
						log::warn("Failed to set EndLevelLayer::showLayer hook priority, node IDs may not work properly");
				}
		}

		void customSetup() {
				EndLevelLayer::customSetup();

				NodeIDs::get()->provide(this);
		}

		void showLayer(bool p0) {
				EndLevelLayer::showLayer(p0);

				for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
						if (auto star = getChildBySpriteFrameName(child, "GJ_bigStar_001.png")) {
								child->setID("star-container");
								star->setID("star-sprite");
								child->getChildByType<CCLabelBMFont>(0)->setID("star-label");
						}
				}
		}
};