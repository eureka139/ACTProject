#pragma once

/**
 *这个宏用于生成变量的Setter和Getter。
 *@param CLASSTYPE 变量的类型
 *@param NAME 变量的名称
 */
#define EXTENSION_GENERATE_SETTER_GETTER(CLASSTYPE,NAME) \
		FORCEINLINE void Set##NAME(CLASSTYPE New##NAME) { NAME = New##NAME; } \
		FORCEINLINE CLASSTYPE Get##NAME() const { return NAME; }