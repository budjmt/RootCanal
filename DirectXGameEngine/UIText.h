#pragma once

#include "UIObject.h"

enum TextJustify : char { LEFT = 'L', RIGHT = 'R', CENTER = 'C' };
struct DrawString {
	std::wstring text = L"";
	vec4 color = vec4(0, 0, 0, 1);
	vec3 position = vec3(); 
	TextJustify justify = LEFT;

	DrawString(std::wstring s, vec4 c, vec3 p, TextJustify j) { text = s; color = c; position = p; justify = j; };
};

class UIText : public UIObject
{
public:
	UIText();

	void drawText(std::wstring str, vec3 position = vec3(), TextJustify justify = TextJustify::LEFT, vec4 color = vec4(0,0,0,1));

	void update(float dt, Mouse* m) { };
	void draw(ID3D11DeviceContext* deviceContext);

private:
	std::vector<DrawString> queuedText;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

