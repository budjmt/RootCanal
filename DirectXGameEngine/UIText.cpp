#include "UIText.h"

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

UIText::UIText() : UIObject()
{
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(DXInfo::getInstance().deviceContext);
}

//origin is top left of the screen
void UIText::drawText(std::wstring text, vec3 pos, TextJustify j, vec4 color) {
	queuedText.push_back(DrawString { text, color, pos, j });
}

void UIText::draw(ID3D11DeviceContext* deviceContext) {
	while (queuedText.size()) {
		auto text = queuedText.back();
		Vector3 pos(text.position.x, text.position.y, text.position.z);
		Vector4 color(text.color.x, text.color.y, text.color.z, text.color.w);
		Vector2 origin = Vector2();
		switch(text.justify) { 
		case TextJustify::LEFT:
			//we're already in the right place
			break;
		case TextJustify::CENTER:
			origin = fonts["arialblack"]->MeasureString(text.text.c_str());
			origin *= 0.5f;
			break;
		case TextJustify::RIGHT:
			origin = fonts["arialblack"]->MeasureString(text.text.c_str());
			break;
		}
		spriteBatch->Begin();
		fonts["arialblack"]->DrawString(spriteBatch.get(), text.text.c_str(), pos, color, 0.f, origin, .75f);
		spriteBatch->End();
		queuedText.pop_back();
	}
}