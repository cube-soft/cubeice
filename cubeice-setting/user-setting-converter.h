#ifndef CUBEICE_USER_SETTING_CONVERTER_H
#define CUBEICE_USER_SETTING_CONVERTER_H

#include <cubeice/user-setting.h>
#include <cubeice/cubeice-ctxdata.h>

namespace cubeice {
	class user_setting_converter {
	public:
		/* ----------------------------------------------------------------- */
		/*
		 *  to_customized_ctx_menu
		 *
		 *  フラグ形式で保持している（カスタマイズされていない）
		 *  コンテキストメニューをカスタマイズ・コンテキストメニューの
		 *  データ形式に変換する．
		 */
		/* ----------------------------------------------------------------- */
		static std::vector<cubeice::user_setting::SUBMENU> to_customized_ctx_menu(std::size_t ctx_menu) {
			std::vector<cubeice::user_setting::SUBMENU> dest;

			for (int i = 0; MenuItem[i].stringA; ++i) {
				if ((ctx_menu & MenuItem[i].dispSetting) != 0) {
#ifdef UNICODE
					cubeice::user_setting::SUBMENU item(MenuItem[i].dispSetting, MenuItem[i].stringW);
#else
					cubeice::user_setting::SUBMENU item(MenuItem[i].dispSetting, MenuItem[i].stringA);
#endif
					item.children = to_customized_ctx_leaf_menu(ctx_menu, MenuItem[i].submenu);
					dest.push_back(item);
				}
			}
			return dest;
		}

	private:
		/* ----------------------------------------------------------------- */
		//  to_customized_ctx_leaf_menu
		/* ----------------------------------------------------------------- */
		static std::vector<cubeice::user_setting::SUBMENU> to_customized_ctx_leaf_menu(std::size_t ctx_menu, const SUB_MENU_ITEM* submenu) {
			std::vector<cubeice::user_setting::SUBMENU> dest;
			for (int i = 0; submenu[i].stringA; ++i) {
				if ((ctx_menu & submenu[i].dispSetting) != 0) {
#ifdef UNICODE
					cubeice::user_setting::SUBMENU item(submenu[i].dispSetting, submenu[i].stringW);
#else
					cubeice::user_setting::SUBMENU item(submenu[i].dispSetting, submenu[i].stringA);
#endif
					dest.push_back(item);
				}
			}
			return dest;
		}
	};
}

#endif // CUBEICE_USER_SETTING_CONVERTER_H
