-- IMGUI_LAYER_1
return [[
typedef void FILE;
typedef struct ImDrawChannel ImDrawChannel;
typedef struct ImDrawCmd ImDrawCmd;
typedef struct ImDrawData ImDrawData;
typedef struct ImDrawList ImDrawList;
typedef struct ImDrawListSharedData ImDrawListSharedData;
typedef struct ImDrawListSplitter ImDrawListSplitter;
typedef struct ImDrawVert ImDrawVert;
typedef struct ImFont ImFont;
typedef struct ImFontAtlas ImFontAtlas;
typedef struct ImFontBuilderIO ImFontBuilderIO;
typedef struct ImFontConfig ImFontConfig;
typedef struct ImFontGlyph ImFontGlyph;
typedef struct ImFontGlyphRangesBuilder ImFontGlyphRangesBuilder;
typedef struct ImColor ImColor;
typedef struct ImGuiContext ImGuiContext;
typedef struct ImGuiIO ImGuiIO;
typedef struct ImGuiInputTextCallbackData ImGuiInputTextCallbackData;
typedef struct ImGuiKeyData ImGuiKeyData;
typedef struct ImGuiListClipper ImGuiListClipper;
typedef struct ImGuiMultiSelectIO ImGuiMultiSelectIO;
typedef struct ImGuiOnceUponAFrame ImGuiOnceUponAFrame;
typedef struct ImGuiPayload ImGuiPayload;
typedef struct ImGuiPlatformIO ImGuiPlatformIO;
typedef struct ImGuiPlatformImeData ImGuiPlatformImeData;
typedef struct ImGuiPlatformMonitor ImGuiPlatformMonitor;
typedef struct ImGuiSelectionBasicStorage ImGuiSelectionBasicStorage;
typedef struct ImGuiSelectionExternalStorage ImGuiSelectionExternalStorage;
typedef struct ImGuiSelectionRequest ImGuiSelectionRequest;
typedef struct ImGuiSizeCallbackData ImGuiSizeCallbackData;
typedef struct ImGuiStorage ImGuiStorage;
typedef struct ImGuiStoragePair ImGuiStoragePair;
typedef struct ImGuiStyle ImGuiStyle;
typedef struct ImGuiTableSortSpecs ImGuiTableSortSpecs;
typedef struct ImGuiTableColumnSortSpecs ImGuiTableColumnSortSpecs;
typedef struct ImGuiTextBuffer ImGuiTextBuffer;
typedef struct ImGuiTextFilter ImGuiTextFilter;
typedef struct ImGuiViewport ImGuiViewport;
typedef struct ImGuiWindowClass ImGuiWindowClass;
typedef struct ImBitVector ImBitVector;
typedef struct ImRect ImRect;
typedef struct ImDrawDataBuilder ImDrawDataBuilder;
typedef struct ImGuiBoxSelectState ImGuiBoxSelectState;
typedef struct ImGuiColorMod ImGuiColorMod;
typedef struct ImGuiContextHook ImGuiContextHook;
typedef struct ImGuiDataVarInfo ImGuiDataVarInfo;
typedef struct ImGuiDataTypeInfo ImGuiDataTypeInfo;
typedef struct ImGuiDockContext ImGuiDockContext;
typedef struct ImGuiDockRequest ImGuiDockRequest;
typedef struct ImGuiDockNode ImGuiDockNode;
typedef struct ImGuiDockNodeSettings ImGuiDockNodeSettings;
typedef struct ImGuiErrorRecoveryState ImGuiErrorRecoveryState;
typedef struct ImGuiGroupData ImGuiGroupData;
typedef struct ImGuiInputTextState ImGuiInputTextState;
typedef struct ImGuiInputTextDeactivateData ImGuiInputTextDeactivateData;
typedef struct ImGuiLastItemData ImGuiLastItemData;
typedef struct ImGuiLocEntry ImGuiLocEntry;
typedef struct ImGuiMenuColumns ImGuiMenuColumns;
typedef struct ImGuiMultiSelectState ImGuiMultiSelectState;
typedef struct ImGuiMultiSelectTempData ImGuiMultiSelectTempData;
typedef struct ImGuiNavItemData ImGuiNavItemData;
typedef struct ImGuiMetricsConfig ImGuiMetricsConfig;
typedef struct ImGuiNextWindowData ImGuiNextWindowData;
typedef struct ImGuiNextItemData ImGuiNextItemData;
typedef struct ImGuiOldColumnData ImGuiOldColumnData;
typedef struct ImGuiOldColumns ImGuiOldColumns;
typedef struct ImGuiPopupData ImGuiPopupData;
typedef struct ImGuiSettingsHandler ImGuiSettingsHandler;
typedef struct ImGuiStyleMod ImGuiStyleMod;
typedef struct ImGuiTabBar ImGuiTabBar;
typedef struct ImGuiTabItem ImGuiTabItem;
typedef struct ImGuiTable ImGuiTable;
typedef struct ImGuiTableHeaderData ImGuiTableHeaderData;
typedef struct ImGuiTableColumn ImGuiTableColumn;
typedef struct ImGuiTableInstanceData ImGuiTableInstanceData;
typedef struct ImGuiTableTempData ImGuiTableTempData;
typedef struct ImGuiTableSettings ImGuiTableSettings;
typedef struct ImGuiTableColumnsSettings ImGuiTableColumnsSettings;
typedef struct ImGuiTreeNodeStackData ImGuiTreeNodeStackData;
typedef struct ImGuiTypingSelectState ImGuiTypingSelectState;
typedef struct ImGuiTypingSelectRequest ImGuiTypingSelectRequest;
typedef struct ImGuiWindow ImGuiWindow;
typedef struct ImGuiWindowDockStyle ImGuiWindowDockStyle;
typedef struct ImGuiWindowTempData ImGuiWindowTempData;
typedef struct ImGuiWindowSettings ImGuiWindowSettings;
typedef struct STB_TexteditState STB_TexteditState;
typedef struct ImVector_const_charPtr {int Size;int Capacity;const char** Data;} ImVector_const_charPtr;
typedef unsigned int ImGuiID;
typedef signed char ImS8;
typedef unsigned char ImU8;
typedef signed short ImS16;
typedef unsigned short ImU16;
typedef signed int ImS32;
typedef unsigned int ImU32;
typedef signed long long ImS64;
typedef unsigned long long ImU64;
struct ImDrawChannel;
struct ImDrawCmd;
struct ImDrawData;
struct ImDrawList;
struct ImDrawListSharedData;
struct ImDrawListSplitter;
struct ImDrawVert;
struct ImFont;
struct ImFontAtlas;
struct ImFontBuilderIO;
struct ImFontConfig;
struct ImFontGlyph;
struct ImFontGlyphRangesBuilder;
struct ImColor;
struct ImGuiContext;
struct ImGuiIO;
struct ImGuiInputTextCallbackData;
struct ImGuiKeyData;
struct ImGuiListClipper;
struct ImGuiMultiSelectIO;
struct ImGuiOnceUponAFrame;
struct ImGuiPayload;
struct ImGuiPlatformIO;
struct ImGuiPlatformImeData;
struct ImGuiPlatformMonitor;
struct ImGuiSelectionBasicStorage;
struct ImGuiSelectionExternalStorage;
struct ImGuiSelectionRequest;
struct ImGuiSizeCallbackData;
struct ImGuiStorage;
struct ImGuiStoragePair;
struct ImGuiStyle;
struct ImGuiTableSortSpecs;
struct ImGuiTableColumnSortSpecs;
struct ImGuiTextBuffer;
struct ImGuiTextFilter;
struct ImGuiViewport;
struct ImGuiWindowClass;
typedef int ImGuiCol;
typedef int ImGuiCond;
typedef int ImGuiDataType;
typedef int ImGuiMouseButton;
typedef int ImGuiMouseCursor;
typedef int ImGuiStyleVar;
typedef int ImGuiTableBgTarget;
typedef int ImDrawFlags;
typedef int ImDrawListFlags;
typedef int ImFontAtlasFlags;
typedef int ImGuiBackendFlags;
typedef int ImGuiButtonFlags;
typedef int ImGuiChildFlags;
typedef int ImGuiColorEditFlags;
typedef int ImGuiConfigFlags;
typedef int ImGuiComboFlags;
typedef int ImGuiDockNodeFlags;
typedef int ImGuiDragDropFlags;
typedef int ImGuiFocusedFlags;
typedef int ImGuiHoveredFlags;
typedef int ImGuiInputFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiItemFlags;
typedef int ImGuiKeyChord;
typedef int ImGuiPopupFlags;
typedef int ImGuiMultiSelectFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiSliderFlags;
typedef int ImGuiTabBarFlags;
typedef int ImGuiTabItemFlags;
typedef int ImGuiTableFlags;
typedef int ImGuiTableColumnFlags;
typedef int ImGuiTableRowFlags;
typedef int ImGuiTreeNodeFlags;
typedef int ImGuiViewportFlags;
typedef int ImGuiWindowFlags;
typedef ImU64 ImTextureID;
typedef unsigned short ImDrawIdx;
typedef unsigned int ImWchar32;
typedef unsigned short ImWchar16;
typedef ImWchar16 ImWchar;
typedef ImS64 ImGuiSelectionUserData;
typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);
typedef void (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);
typedef void* (*ImGuiMemAllocFunc)(size_t sz, void* user_data);
typedef void (*ImGuiMemFreeFunc)(void* ptr, void* user_data);
typedef struct ImVec2 ImVec2;
struct ImVec2
{
    float x, y;
};
typedef struct ImVec4 ImVec4;
struct ImVec4
{
    float x, y, z, w;
};
typedef enum {
    ImGuiWindowFlags_None = 0,
    ImGuiWindowFlags_NoTitleBar = 1 << 0,
    ImGuiWindowFlags_NoResize = 1 << 1,
    ImGuiWindowFlags_NoMove = 1 << 2,
    ImGuiWindowFlags_NoScrollbar = 1 << 3,
    ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,
    ImGuiWindowFlags_NoCollapse = 1 << 5,
    ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
    ImGuiWindowFlags_NoBackground = 1 << 7,
    ImGuiWindowFlags_NoSavedSettings = 1 << 8,
    ImGuiWindowFlags_NoMouseInputs = 1 << 9,
    ImGuiWindowFlags_MenuBar = 1 << 10,
    ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,
    ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,
    ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,
    ImGuiWindowFlags_NoNavInputs = 1 << 16,
    ImGuiWindowFlags_NoNavFocus = 1 << 17,
    ImGuiWindowFlags_UnsavedDocument = 1 << 18,
    ImGuiWindowFlags_NoDocking = 1 << 19,
    ImGuiWindowFlags_NoNav = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_ChildWindow = 1 << 24,
    ImGuiWindowFlags_Tooltip = 1 << 25,
    ImGuiWindowFlags_Popup = 1 << 26,
    ImGuiWindowFlags_Modal = 1 << 27,
    ImGuiWindowFlags_ChildMenu = 1 << 28,
    ImGuiWindowFlags_DockNodeHost = 1 << 29,
}ImGuiWindowFlags_;
typedef enum {
    ImGuiChildFlags_None = 0,
    ImGuiChildFlags_Borders = 1 << 0,
    ImGuiChildFlags_AlwaysUseWindowPadding = 1 << 1,
    ImGuiChildFlags_ResizeX = 1 << 2,
    ImGuiChildFlags_ResizeY = 1 << 3,
    ImGuiChildFlags_AutoResizeX = 1 << 4,
    ImGuiChildFlags_AutoResizeY = 1 << 5,
    ImGuiChildFlags_AlwaysAutoResize = 1 << 6,
    ImGuiChildFlags_FrameStyle = 1 << 7,
    ImGuiChildFlags_NavFlattened = 1 << 8,
}ImGuiChildFlags_;
typedef enum {
    ImGuiItemFlags_None = 0,
    ImGuiItemFlags_NoTabStop = 1 << 0,
    ImGuiItemFlags_NoNav = 1 << 1,
    ImGuiItemFlags_NoNavDefaultFocus = 1 << 2,
    ImGuiItemFlags_ButtonRepeat = 1 << 3,
    ImGuiItemFlags_AutoClosePopups = 1 << 4,
    ImGuiItemFlags_AllowDuplicateId = 1 << 5,
}ImGuiItemFlags_;
typedef enum {
    ImGuiInputTextFlags_None = 0,
    ImGuiInputTextFlags_CharsDecimal = 1 << 0,
    ImGuiInputTextFlags_CharsHexadecimal = 1 << 1,
    ImGuiInputTextFlags_CharsScientific = 1 << 2,
    ImGuiInputTextFlags_CharsUppercase = 1 << 3,
    ImGuiInputTextFlags_CharsNoBlank = 1 << 4,
    ImGuiInputTextFlags_AllowTabInput = 1 << 5,
    ImGuiInputTextFlags_EnterReturnsTrue = 1 << 6,
    ImGuiInputTextFlags_EscapeClearsAll = 1 << 7,
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 8,
    ImGuiInputTextFlags_ReadOnly = 1 << 9,
    ImGuiInputTextFlags_Password = 1 << 10,
    ImGuiInputTextFlags_AlwaysOverwrite = 1 << 11,
    ImGuiInputTextFlags_AutoSelectAll = 1 << 12,
    ImGuiInputTextFlags_ParseEmptyRefVal = 1 << 13,
    ImGuiInputTextFlags_DisplayEmptyRefVal = 1 << 14,
    ImGuiInputTextFlags_NoHorizontalScroll = 1 << 15,
    ImGuiInputTextFlags_NoUndoRedo = 1 << 16,
    ImGuiInputTextFlags_CallbackCompletion = 1 << 17,
    ImGuiInputTextFlags_CallbackHistory = 1 << 18,
    ImGuiInputTextFlags_CallbackAlways = 1 << 19,
    ImGuiInputTextFlags_CallbackCharFilter = 1 << 20,
    ImGuiInputTextFlags_CallbackResize = 1 << 21,
    ImGuiInputTextFlags_CallbackEdit = 1 << 22,
}ImGuiInputTextFlags_;
typedef enum {
    ImGuiTreeNodeFlags_None = 0,
    ImGuiTreeNodeFlags_Selected = 1 << 0,
    ImGuiTreeNodeFlags_Framed = 1 << 1,
    ImGuiTreeNodeFlags_AllowOverlap = 1 << 2,
    ImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3,
    ImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4,
    ImGuiTreeNodeFlags_DefaultOpen = 1 << 5,
    ImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6,
    ImGuiTreeNodeFlags_OpenOnArrow = 1 << 7,
    ImGuiTreeNodeFlags_Leaf = 1 << 8,
    ImGuiTreeNodeFlags_Bullet = 1 << 9,
    ImGuiTreeNodeFlags_FramePadding = 1 << 10,
    ImGuiTreeNodeFlags_SpanAvailWidth = 1 << 11,
    ImGuiTreeNodeFlags_SpanFullWidth = 1 << 12,
    ImGuiTreeNodeFlags_SpanTextWidth = 1 << 13,
    ImGuiTreeNodeFlags_SpanAllColumns = 1 << 14,
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 15,
    ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,
}ImGuiTreeNodeFlags_;
typedef enum {
    ImGuiPopupFlags_None = 0,
    ImGuiPopupFlags_MouseButtonLeft = 0,
    ImGuiPopupFlags_MouseButtonRight = 1,
    ImGuiPopupFlags_MouseButtonMiddle = 2,
    ImGuiPopupFlags_MouseButtonMask_ = 0x1F,
    ImGuiPopupFlags_MouseButtonDefault_ = 1,
    ImGuiPopupFlags_NoReopen = 1 << 5,
    ImGuiPopupFlags_NoOpenOverExistingPopup = 1 << 7,
    ImGuiPopupFlags_NoOpenOverItems = 1 << 8,
    ImGuiPopupFlags_AnyPopupId = 1 << 10,
    ImGuiPopupFlags_AnyPopupLevel = 1 << 11,
    ImGuiPopupFlags_AnyPopup = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel,
}ImGuiPopupFlags_;
typedef enum {
    ImGuiSelectableFlags_None = 0,
    ImGuiSelectableFlags_NoAutoClosePopups = 1 << 0,
    ImGuiSelectableFlags_SpanAllColumns = 1 << 1,
    ImGuiSelectableFlags_AllowDoubleClick = 1 << 2,
    ImGuiSelectableFlags_Disabled = 1 << 3,
    ImGuiSelectableFlags_AllowOverlap = 1 << 4,
    ImGuiSelectableFlags_Highlight = 1 << 5,
}ImGuiSelectableFlags_;
typedef enum {
    ImGuiComboFlags_None = 0,
    ImGuiComboFlags_PopupAlignLeft = 1 << 0,
    ImGuiComboFlags_HeightSmall = 1 << 1,
    ImGuiComboFlags_HeightRegular = 1 << 2,
    ImGuiComboFlags_HeightLarge = 1 << 3,
    ImGuiComboFlags_HeightLargest = 1 << 4,
    ImGuiComboFlags_NoArrowButton = 1 << 5,
    ImGuiComboFlags_NoPreview = 1 << 6,
    ImGuiComboFlags_WidthFitPreview = 1 << 7,
    ImGuiComboFlags_HeightMask_ = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
}ImGuiComboFlags_;
typedef enum {
    ImGuiTabBarFlags_None = 0,
    ImGuiTabBarFlags_Reorderable = 1 << 0,
    ImGuiTabBarFlags_AutoSelectNewTabs = 1 << 1,
    ImGuiTabBarFlags_TabListPopupButton = 1 << 2,
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,
    ImGuiTabBarFlags_NoTabListScrollingButtons = 1 << 4,
    ImGuiTabBarFlags_NoTooltip = 1 << 5,
    ImGuiTabBarFlags_DrawSelectedOverline = 1 << 6,
    ImGuiTabBarFlags_FittingPolicyResizeDown = 1 << 7,
    ImGuiTabBarFlags_FittingPolicyScroll = 1 << 8,
    ImGuiTabBarFlags_FittingPolicyMask_ = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_ = ImGuiTabBarFlags_FittingPolicyResizeDown,
}ImGuiTabBarFlags_;
typedef enum {
    ImGuiTabItemFlags_None = 0,
    ImGuiTabItemFlags_UnsavedDocument = 1 << 0,
    ImGuiTabItemFlags_SetSelected = 1 << 1,
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,
    ImGuiTabItemFlags_NoPushId = 1 << 3,
    ImGuiTabItemFlags_NoTooltip = 1 << 4,
    ImGuiTabItemFlags_NoReorder = 1 << 5,
    ImGuiTabItemFlags_Leading = 1 << 6,
    ImGuiTabItemFlags_Trailing = 1 << 7,
    ImGuiTabItemFlags_NoAssumedClosure = 1 << 8,
}ImGuiTabItemFlags_;
typedef enum {
    ImGuiFocusedFlags_None = 0,
    ImGuiFocusedFlags_ChildWindows = 1 << 0,
    ImGuiFocusedFlags_RootWindow = 1 << 1,
    ImGuiFocusedFlags_AnyWindow = 1 << 2,
    ImGuiFocusedFlags_NoPopupHierarchy = 1 << 3,
    ImGuiFocusedFlags_DockHierarchy = 1 << 4,
    ImGuiFocusedFlags_RootAndChildWindows = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows,
}ImGuiFocusedFlags_;
typedef enum {
    ImGuiHoveredFlags_None = 0,
    ImGuiHoveredFlags_ChildWindows = 1 << 0,
    ImGuiHoveredFlags_RootWindow = 1 << 1,
    ImGuiHoveredFlags_AnyWindow = 1 << 2,
    ImGuiHoveredFlags_NoPopupHierarchy = 1 << 3,
    ImGuiHoveredFlags_DockHierarchy = 1 << 4,
    ImGuiHoveredFlags_AllowWhenBlockedByPopup = 1 << 5,
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,
    ImGuiHoveredFlags_AllowWhenOverlappedByItem = 1 << 8,
    ImGuiHoveredFlags_AllowWhenOverlappedByWindow = 1 << 9,
    ImGuiHoveredFlags_AllowWhenDisabled = 1 << 10,
    ImGuiHoveredFlags_NoNavOverride = 1 << 11,
    ImGuiHoveredFlags_AllowWhenOverlapped = ImGuiHoveredFlags_AllowWhenOverlappedByItem | ImGuiHoveredFlags_AllowWhenOverlappedByWindow,
    ImGuiHoveredFlags_RectOnly = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows,
    ImGuiHoveredFlags_ForTooltip = 1 << 12,
    ImGuiHoveredFlags_Stationary = 1 << 13,
    ImGuiHoveredFlags_DelayNone = 1 << 14,
    ImGuiHoveredFlags_DelayShort = 1 << 15,
    ImGuiHoveredFlags_DelayNormal = 1 << 16,
    ImGuiHoveredFlags_NoSharedDelay = 1 << 17,
}ImGuiHoveredFlags_;
typedef enum {
    ImGuiDockNodeFlags_None = 0,
    ImGuiDockNodeFlags_KeepAliveOnly = 1 << 0,
    ImGuiDockNodeFlags_NoDockingOverCentralNode = 1 << 2,
    ImGuiDockNodeFlags_PassthruCentralNode = 1 << 3,
    ImGuiDockNodeFlags_NoDockingSplit = 1 << 4,
    ImGuiDockNodeFlags_NoResize = 1 << 5,
    ImGuiDockNodeFlags_AutoHideTabBar = 1 << 6,
    ImGuiDockNodeFlags_NoUndocking = 1 << 7,
}ImGuiDockNodeFlags_;
typedef enum {
    ImGuiDragDropFlags_None = 0,
    ImGuiDragDropFlags_SourceNoPreviewTooltip = 1 << 0,
    ImGuiDragDropFlags_SourceNoDisableHover = 1 << 1,
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,
    ImGuiDragDropFlags_SourceAllowNullID = 1 << 3,
    ImGuiDragDropFlags_SourceExtern = 1 << 4,
    ImGuiDragDropFlags_PayloadAutoExpire = 1 << 5,
    ImGuiDragDropFlags_PayloadNoCrossContext = 1 << 6,
    ImGuiDragDropFlags_PayloadNoCrossProcess = 1 << 7,
    ImGuiDragDropFlags_AcceptBeforeDelivery = 1 << 10,
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect = 1 << 11,
    ImGuiDragDropFlags_AcceptNoPreviewTooltip = 1 << 12,
    ImGuiDragDropFlags_AcceptPeekOnly = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect,
}ImGuiDragDropFlags_;
typedef enum {
    ImGuiDataType_S8,
    ImGuiDataType_U8,
    ImGuiDataType_S16,
    ImGuiDataType_U16,
    ImGuiDataType_S32,
    ImGuiDataType_U32,
    ImGuiDataType_S64,
    ImGuiDataType_U64,
    ImGuiDataType_Float,
    ImGuiDataType_Double,
    ImGuiDataType_Bool,
    ImGuiDataType_COUNT
}ImGuiDataType_;
typedef enum {
ImGuiDir_None=-1,
ImGuiDir_Left=0,
ImGuiDir_Right=1,
ImGuiDir_Up=2,
ImGuiDir_Down=3,
ImGuiDir_COUNT=4,
}ImGuiDir;
typedef enum {
ImGuiSortDirection_None=0,
ImGuiSortDirection_Ascending=1,
ImGuiSortDirection_Descending=2,
}ImGuiSortDirection;
typedef enum {
ImGuiKey_None=0,
ImGuiKey_Tab=512,
ImGuiKey_LeftArrow=513,
ImGuiKey_RightArrow=514,
ImGuiKey_UpArrow=515,
ImGuiKey_DownArrow=516,
ImGuiKey_PageUp=517,
ImGuiKey_PageDown=518,
ImGuiKey_Home=519,
ImGuiKey_End=520,
ImGuiKey_Insert=521,
ImGuiKey_Delete=522,
ImGuiKey_Backspace=523,
ImGuiKey_Space=524,
ImGuiKey_Enter=525,
ImGuiKey_Escape=526,
ImGuiKey_LeftCtrl=527,
ImGuiKey_LeftShift=528,
ImGuiKey_LeftAlt=529,
ImGuiKey_LeftSuper=530,
ImGuiKey_RightCtrl=531,
ImGuiKey_RightShift=532,
ImGuiKey_RightAlt=533,
ImGuiKey_RightSuper=534,
ImGuiKey_Menu=535,
ImGuiKey_0=536,
ImGuiKey_1=537,
ImGuiKey_2=538,
ImGuiKey_3=539,
ImGuiKey_4=540,
ImGuiKey_5=541,
ImGuiKey_6=542,
ImGuiKey_7=543,
ImGuiKey_8=544,
ImGuiKey_9=545,
ImGuiKey_A=546,
ImGuiKey_B=547,
ImGuiKey_C=548,
ImGuiKey_D=549,
ImGuiKey_E=550,
ImGuiKey_F=551,
ImGuiKey_G=552,
ImGuiKey_H=553,
ImGuiKey_I=554,
ImGuiKey_J=555,
ImGuiKey_K=556,
ImGuiKey_L=557,
ImGuiKey_M=558,
ImGuiKey_N=559,
ImGuiKey_O=560,
ImGuiKey_P=561,
ImGuiKey_Q=562,
ImGuiKey_R=563,
ImGuiKey_S=564,
ImGuiKey_T=565,
ImGuiKey_U=566,
ImGuiKey_V=567,
ImGuiKey_W=568,
ImGuiKey_X=569,
ImGuiKey_Y=570,
ImGuiKey_Z=571,
ImGuiKey_F1=572,
ImGuiKey_F2=573,
ImGuiKey_F3=574,
ImGuiKey_F4=575,
ImGuiKey_F5=576,
ImGuiKey_F6=577,
ImGuiKey_F7=578,
ImGuiKey_F8=579,
ImGuiKey_F9=580,
ImGuiKey_F10=581,
ImGuiKey_F11=582,
ImGuiKey_F12=583,
ImGuiKey_F13=584,
ImGuiKey_F14=585,
ImGuiKey_F15=586,
ImGuiKey_F16=587,
ImGuiKey_F17=588,
ImGuiKey_F18=589,
ImGuiKey_F19=590,
ImGuiKey_F20=591,
ImGuiKey_F21=592,
ImGuiKey_F22=593,
ImGuiKey_F23=594,
ImGuiKey_F24=595,
ImGuiKey_Apostrophe=596,
ImGuiKey_Comma=597,
ImGuiKey_Minus=598,
ImGuiKey_Period=599,
ImGuiKey_Slash=600,
ImGuiKey_Semicolon=601,
ImGuiKey_Equal=602,
ImGuiKey_LeftBracket=603,
ImGuiKey_Backslash=604,
ImGuiKey_RightBracket=605,
ImGuiKey_GraveAccent=606,
ImGuiKey_CapsLock=607,
ImGuiKey_ScrollLock=608,
ImGuiKey_NumLock=609,
ImGuiKey_PrintScreen=610,
ImGuiKey_Pause=611,
ImGuiKey_Keypad0=612,
ImGuiKey_Keypad1=613,
ImGuiKey_Keypad2=614,
ImGuiKey_Keypad3=615,
ImGuiKey_Keypad4=616,
ImGuiKey_Keypad5=617,
ImGuiKey_Keypad6=618,
ImGuiKey_Keypad7=619,
ImGuiKey_Keypad8=620,
ImGuiKey_Keypad9=621,
ImGuiKey_KeypadDecimal=622,
ImGuiKey_KeypadDivide=623,
ImGuiKey_KeypadMultiply=624,
ImGuiKey_KeypadSubtract=625,
ImGuiKey_KeypadAdd=626,
ImGuiKey_KeypadEnter=627,
ImGuiKey_KeypadEqual=628,
ImGuiKey_AppBack=629,
ImGuiKey_AppForward=630,
ImGuiKey_GamepadStart=631,
ImGuiKey_GamepadBack=632,
ImGuiKey_GamepadFaceLeft=633,
ImGuiKey_GamepadFaceRight=634,
ImGuiKey_GamepadFaceUp=635,
ImGuiKey_GamepadFaceDown=636,
ImGuiKey_GamepadDpadLeft=637,
ImGuiKey_GamepadDpadRight=638,
ImGuiKey_GamepadDpadUp=639,
ImGuiKey_GamepadDpadDown=640,
ImGuiKey_GamepadL1=641,
ImGuiKey_GamepadR1=642,
ImGuiKey_GamepadL2=643,
ImGuiKey_GamepadR2=644,
ImGuiKey_GamepadL3=645,
ImGuiKey_GamepadR3=646,
ImGuiKey_GamepadLStickLeft=647,
ImGuiKey_GamepadLStickRight=648,
ImGuiKey_GamepadLStickUp=649,
ImGuiKey_GamepadLStickDown=650,
ImGuiKey_GamepadRStickLeft=651,
ImGuiKey_GamepadRStickRight=652,
ImGuiKey_GamepadRStickUp=653,
ImGuiKey_GamepadRStickDown=654,
ImGuiKey_MouseLeft=655,
ImGuiKey_MouseRight=656,
ImGuiKey_MouseMiddle=657,
ImGuiKey_MouseX1=658,
ImGuiKey_MouseX2=659,
ImGuiKey_MouseWheelX=660,
ImGuiKey_MouseWheelY=661,
ImGuiKey_ReservedForModCtrl=662,
ImGuiKey_ReservedForModShift=663,
ImGuiKey_ReservedForModAlt=664,
ImGuiKey_ReservedForModSuper=665,
ImGuiKey_COUNT=666,
ImGuiMod_None=0,
ImGuiMod_Ctrl=1 << 12,
ImGuiMod_Shift=1 << 13,
ImGuiMod_Alt=1 << 14,
ImGuiMod_Super=1 << 15,
ImGuiMod_Mask_=0xF000,
ImGuiKey_NamedKey_BEGIN=512,
ImGuiKey_NamedKey_END=ImGuiKey_COUNT,
ImGuiKey_NamedKey_COUNT=ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
ImGuiKey_KeysData_SIZE=ImGuiKey_NamedKey_COUNT,
ImGuiKey_KeysData_OFFSET=ImGuiKey_NamedKey_BEGIN,
}ImGuiKey;
typedef enum {
    ImGuiInputFlags_None = 0,
    ImGuiInputFlags_Repeat = 1 << 0,
    ImGuiInputFlags_RouteActive = 1 << 10,
    ImGuiInputFlags_RouteFocused = 1 << 11,
    ImGuiInputFlags_RouteGlobal = 1 << 12,
    ImGuiInputFlags_RouteAlways = 1 << 13,
    ImGuiInputFlags_RouteOverFocused = 1 << 14,
    ImGuiInputFlags_RouteOverActive = 1 << 15,
    ImGuiInputFlags_RouteUnlessBgFocused = 1 << 16,
    ImGuiInputFlags_RouteFromRootWindow = 1 << 17,
    ImGuiInputFlags_Tooltip = 1 << 18,
}ImGuiInputFlags_;
typedef enum {
    ImGuiConfigFlags_None = 0,
    ImGuiConfigFlags_NavEnableKeyboard = 1 << 0,
    ImGuiConfigFlags_NavEnableGamepad = 1 << 1,
    ImGuiConfigFlags_NoMouse = 1 << 4,
    ImGuiConfigFlags_NoMouseCursorChange = 1 << 5,
    ImGuiConfigFlags_NoKeyboard = 1 << 6,
    ImGuiConfigFlags_DockingEnable = 1 << 7,
    ImGuiConfigFlags_ViewportsEnable = 1 << 10,
    ImGuiConfigFlags_DpiEnableScaleViewports= 1 << 14,
    ImGuiConfigFlags_DpiEnableScaleFonts = 1 << 15,
    ImGuiConfigFlags_IsSRGB = 1 << 20,
    ImGuiConfigFlags_IsTouchScreen = 1 << 21,
}ImGuiConfigFlags_;
typedef enum {
    ImGuiBackendFlags_None = 0,
    ImGuiBackendFlags_HasGamepad = 1 << 0,
    ImGuiBackendFlags_HasMouseCursors = 1 << 1,
    ImGuiBackendFlags_HasSetMousePos = 1 << 2,
    ImGuiBackendFlags_RendererHasVtxOffset = 1 << 3,
    ImGuiBackendFlags_PlatformHasViewports = 1 << 10,
    ImGuiBackendFlags_HasMouseHoveredViewport=1 << 11,
    ImGuiBackendFlags_RendererHasViewports = 1 << 12,
}ImGuiBackendFlags_;
typedef enum {
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,
    ImGuiCol_ChildBg,
    ImGuiCol_PopupBg,
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_TabHovered,
    ImGuiCol_Tab,
    ImGuiCol_TabSelected,
    ImGuiCol_TabSelectedOverline,
    ImGuiCol_TabDimmed,
    ImGuiCol_TabDimmedSelected,
    ImGuiCol_TabDimmedSelectedOverline,
    ImGuiCol_DockingPreview,
    ImGuiCol_DockingEmptyBg,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,
    ImGuiCol_TableBorderStrong,
    ImGuiCol_TableBorderLight,
    ImGuiCol_TableRowBg,
    ImGuiCol_TableRowBgAlt,
    ImGuiCol_TextLink,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavCursor,
    ImGuiCol_NavWindowingHighlight,
    ImGuiCol_NavWindowingDimBg,
    ImGuiCol_ModalWindowDimBg,
    ImGuiCol_COUNT,
}ImGuiCol_;
typedef enum {
    ImGuiStyleVar_Alpha,
    ImGuiStyleVar_DisabledAlpha,
    ImGuiStyleVar_WindowPadding,
    ImGuiStyleVar_WindowRounding,
    ImGuiStyleVar_WindowBorderSize,
    ImGuiStyleVar_WindowMinSize,
    ImGuiStyleVar_WindowTitleAlign,
    ImGuiStyleVar_ChildRounding,
    ImGuiStyleVar_ChildBorderSize,
    ImGuiStyleVar_PopupRounding,
    ImGuiStyleVar_PopupBorderSize,
    ImGuiStyleVar_FramePadding,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_FrameBorderSize,
    ImGuiStyleVar_ItemSpacing,
    ImGuiStyleVar_ItemInnerSpacing,
    ImGuiStyleVar_IndentSpacing,
    ImGuiStyleVar_CellPadding,
    ImGuiStyleVar_ScrollbarSize,
    ImGuiStyleVar_ScrollbarRounding,
    ImGuiStyleVar_GrabMinSize,
    ImGuiStyleVar_GrabRounding,
    ImGuiStyleVar_TabRounding,
    ImGuiStyleVar_TabBorderSize,
    ImGuiStyleVar_TabBarBorderSize,
    ImGuiStyleVar_TabBarOverlineSize,
    ImGuiStyleVar_TableAngledHeadersAngle,
    ImGuiStyleVar_TableAngledHeadersTextAlign,
    ImGuiStyleVar_ButtonTextAlign,
    ImGuiStyleVar_SelectableTextAlign,
    ImGuiStyleVar_SeparatorTextBorderSize,
    ImGuiStyleVar_SeparatorTextAlign,
    ImGuiStyleVar_SeparatorTextPadding,
    ImGuiStyleVar_DockingSeparatorSize,
    ImGuiStyleVar_COUNT
}ImGuiStyleVar_;
typedef enum {
    ImGuiButtonFlags_None = 0,
    ImGuiButtonFlags_MouseButtonLeft = 1 << 0,
    ImGuiButtonFlags_MouseButtonRight = 1 << 1,
    ImGuiButtonFlags_MouseButtonMiddle = 1 << 2,
    ImGuiButtonFlags_MouseButtonMask_ = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle,
    ImGuiButtonFlags_EnableNav = 1 << 3,
}ImGuiButtonFlags_;
typedef enum {
    ImGuiColorEditFlags_None = 0,
    ImGuiColorEditFlags_NoAlpha = 1 << 1,
    ImGuiColorEditFlags_NoPicker = 1 << 2,
    ImGuiColorEditFlags_NoOptions = 1 << 3,
    ImGuiColorEditFlags_NoSmallPreview = 1 << 4,
    ImGuiColorEditFlags_NoInputs = 1 << 5,
    ImGuiColorEditFlags_NoTooltip = 1 << 6,
    ImGuiColorEditFlags_NoLabel = 1 << 7,
    ImGuiColorEditFlags_NoSidePreview = 1 << 8,
    ImGuiColorEditFlags_NoDragDrop = 1 << 9,
    ImGuiColorEditFlags_NoBorder = 1 << 10,
    ImGuiColorEditFlags_AlphaBar = 1 << 16,
    ImGuiColorEditFlags_AlphaPreview = 1 << 17,
    ImGuiColorEditFlags_AlphaPreviewHalf= 1 << 18,
    ImGuiColorEditFlags_HDR = 1 << 19,
    ImGuiColorEditFlags_DisplayRGB = 1 << 20,
    ImGuiColorEditFlags_DisplayHSV = 1 << 21,
    ImGuiColorEditFlags_DisplayHex = 1 << 22,
    ImGuiColorEditFlags_Uint8 = 1 << 23,
    ImGuiColorEditFlags_Float = 1 << 24,
    ImGuiColorEditFlags_PickerHueBar = 1 << 25,
    ImGuiColorEditFlags_PickerHueWheel = 1 << 26,
    ImGuiColorEditFlags_InputRGB = 1 << 27,
    ImGuiColorEditFlags_InputHSV = 1 << 28,
    ImGuiColorEditFlags_DefaultOptions_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_DisplayMask_ = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex,
    ImGuiColorEditFlags_DataTypeMask_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags_PickerMask_ = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_InputMask_ = ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV,
}ImGuiColorEditFlags_;
typedef enum {
    ImGuiSliderFlags_None = 0,
    ImGuiSliderFlags_Logarithmic = 1 << 5,
    ImGuiSliderFlags_NoRoundToFormat = 1 << 6,
    ImGuiSliderFlags_NoInput = 1 << 7,
    ImGuiSliderFlags_WrapAround = 1 << 8,
    ImGuiSliderFlags_ClampOnInput = 1 << 9,
    ImGuiSliderFlags_ClampZeroRange = 1 << 10,
    ImGuiSliderFlags_AlwaysClamp = ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_ClampZeroRange,
    ImGuiSliderFlags_InvalidMask_ = 0x7000000F,
}ImGuiSliderFlags_;
typedef enum {
    ImGuiMouseButton_Left = 0,
    ImGuiMouseButton_Right = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT = 5
}ImGuiMouseButton_;
typedef enum {
    ImGuiMouseCursor_None = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,
    ImGuiMouseCursor_ResizeAll,
    ImGuiMouseCursor_ResizeNS,
    ImGuiMouseCursor_ResizeEW,
    ImGuiMouseCursor_ResizeNESW,
    ImGuiMouseCursor_ResizeNWSE,
    ImGuiMouseCursor_Hand,
    ImGuiMouseCursor_NotAllowed,
    ImGuiMouseCursor_COUNT
}ImGuiMouseCursor_;
typedef enum {
ImGuiMouseSource_Mouse=0,
ImGuiMouseSource_TouchScreen=1,
ImGuiMouseSource_Pen=2,
ImGuiMouseSource_COUNT=3,
}ImGuiMouseSource;
typedef enum {
    ImGuiCond_None = 0,
    ImGuiCond_Always = 1 << 0,
    ImGuiCond_Once = 1 << 1,
    ImGuiCond_FirstUseEver = 1 << 2,
    ImGuiCond_Appearing = 1 << 3,
}ImGuiCond_;
typedef enum {
    ImGuiTableFlags_None = 0,
    ImGuiTableFlags_Resizable = 1 << 0,
    ImGuiTableFlags_Reorderable = 1 << 1,
    ImGuiTableFlags_Hideable = 1 << 2,
    ImGuiTableFlags_Sortable = 1 << 3,
    ImGuiTableFlags_NoSavedSettings = 1 << 4,
    ImGuiTableFlags_ContextMenuInBody = 1 << 5,
    ImGuiTableFlags_RowBg = 1 << 6,
    ImGuiTableFlags_BordersInnerH = 1 << 7,
    ImGuiTableFlags_BordersOuterH = 1 << 8,
    ImGuiTableFlags_BordersInnerV = 1 << 9,
    ImGuiTableFlags_BordersOuterV = 1 << 10,
    ImGuiTableFlags_BordersH = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH,
    ImGuiTableFlags_BordersV = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV,
    ImGuiTableFlags_BordersInner = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH,
    ImGuiTableFlags_BordersOuter = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH,
    ImGuiTableFlags_Borders = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter,
    ImGuiTableFlags_NoBordersInBody = 1 << 11,
    ImGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,
    ImGuiTableFlags_SizingFixedFit = 1 << 13,
    ImGuiTableFlags_SizingFixedSame = 2 << 13,
    ImGuiTableFlags_SizingStretchProp = 3 << 13,
    ImGuiTableFlags_SizingStretchSame = 4 << 13,
    ImGuiTableFlags_NoHostExtendX = 1 << 16,
    ImGuiTableFlags_NoHostExtendY = 1 << 17,
    ImGuiTableFlags_NoKeepColumnsVisible = 1 << 18,
    ImGuiTableFlags_PreciseWidths = 1 << 19,
    ImGuiTableFlags_NoClip = 1 << 20,
    ImGuiTableFlags_PadOuterX = 1 << 21,
    ImGuiTableFlags_NoPadOuterX = 1 << 22,
    ImGuiTableFlags_NoPadInnerX = 1 << 23,
    ImGuiTableFlags_ScrollX = 1 << 24,
    ImGuiTableFlags_ScrollY = 1 << 25,
    ImGuiTableFlags_SortMulti = 1 << 26,
    ImGuiTableFlags_SortTristate = 1 << 27,
    ImGuiTableFlags_HighlightHoveredColumn = 1 << 28,
    ImGuiTableFlags_SizingMask_ = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame,
}ImGuiTableFlags_;
typedef enum {
    ImGuiTableColumnFlags_None = 0,
    ImGuiTableColumnFlags_Disabled = 1 << 0,
    ImGuiTableColumnFlags_DefaultHide = 1 << 1,
    ImGuiTableColumnFlags_DefaultSort = 1 << 2,
    ImGuiTableColumnFlags_WidthStretch = 1 << 3,
    ImGuiTableColumnFlags_WidthFixed = 1 << 4,
    ImGuiTableColumnFlags_NoResize = 1 << 5,
    ImGuiTableColumnFlags_NoReorder = 1 << 6,
    ImGuiTableColumnFlags_NoHide = 1 << 7,
    ImGuiTableColumnFlags_NoClip = 1 << 8,
    ImGuiTableColumnFlags_NoSort = 1 << 9,
    ImGuiTableColumnFlags_NoSortAscending = 1 << 10,
    ImGuiTableColumnFlags_NoSortDescending = 1 << 11,
    ImGuiTableColumnFlags_NoHeaderLabel = 1 << 12,
    ImGuiTableColumnFlags_NoHeaderWidth = 1 << 13,
    ImGuiTableColumnFlags_PreferSortAscending = 1 << 14,
    ImGuiTableColumnFlags_PreferSortDescending = 1 << 15,
    ImGuiTableColumnFlags_IndentEnable = 1 << 16,
    ImGuiTableColumnFlags_IndentDisable = 1 << 17,
    ImGuiTableColumnFlags_AngledHeader = 1 << 18,
    ImGuiTableColumnFlags_IsEnabled = 1 << 24,
    ImGuiTableColumnFlags_IsVisible = 1 << 25,
    ImGuiTableColumnFlags_IsSorted = 1 << 26,
    ImGuiTableColumnFlags_IsHovered = 1 << 27,
    ImGuiTableColumnFlags_WidthMask_ = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed,
    ImGuiTableColumnFlags_IndentMask_ = ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable,
    ImGuiTableColumnFlags_StatusMask_ = ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered,
    ImGuiTableColumnFlags_NoDirectResize_ = 1 << 30,
}ImGuiTableColumnFlags_;
typedef enum {
    ImGuiTableRowFlags_None = 0,
    ImGuiTableRowFlags_Headers = 1 << 0,
}ImGuiTableRowFlags_;
typedef enum {
    ImGuiTableBgTarget_None = 0,
    ImGuiTableBgTarget_RowBg0 = 1,
    ImGuiTableBgTarget_RowBg1 = 2,
    ImGuiTableBgTarget_CellBg = 3,
}ImGuiTableBgTarget_;
struct ImGuiTableSortSpecs
{
    const ImGuiTableColumnSortSpecs* Specs;
    int SpecsCount;
   _Bool         SpecsDirty;
};
struct ImGuiTableColumnSortSpecs
{
    ImGuiID ColumnUserID;
    ImS16 ColumnIndex;
    ImS16 SortOrder;
    ImGuiSortDirection SortDirection;
};
struct ImGuiStyle
{
    float Alpha;
    float DisabledAlpha;
    ImVec2 WindowPadding;
    float WindowRounding;
    float WindowBorderSize;
    ImVec2 WindowMinSize;
    ImVec2 WindowTitleAlign;
    ImGuiDir WindowMenuButtonPosition;
    float ChildRounding;
    float ChildBorderSize;
    float PopupRounding;
    float PopupBorderSize;
    ImVec2 FramePadding;
    float FrameRounding;
    float FrameBorderSize;
    ImVec2 ItemSpacing;
    ImVec2 ItemInnerSpacing;
    ImVec2 CellPadding;
    ImVec2 TouchExtraPadding;
    float IndentSpacing;
    float ColumnsMinSpacing;
    float ScrollbarSize;
    float ScrollbarRounding;
    float GrabMinSize;
    float GrabRounding;
    float LogSliderDeadzone;
    float TabRounding;
    float TabBorderSize;
    float TabMinWidthForCloseButton;
    float TabBarBorderSize;
    float TabBarOverlineSize;
    float TableAngledHeadersAngle;
    ImVec2 TableAngledHeadersTextAlign;
    ImGuiDir ColorButtonPosition;
    ImVec2 ButtonTextAlign;
    ImVec2 SelectableTextAlign;
    float SeparatorTextBorderSize;
    ImVec2 SeparatorTextAlign;
    ImVec2 SeparatorTextPadding;
    ImVec2 DisplayWindowPadding;
    ImVec2 DisplaySafeAreaPadding;
    float DockingSeparatorSize;
    float MouseCursorScale;
   _Bool         AntiAliasedLines;
   _Bool         AntiAliasedLinesUseTex;
   _Bool         AntiAliasedFill;
    float CurveTessellationTol;
    float CircleTessellationMaxError;
    ImVec4 Colors[ImGuiCol_COUNT];
    float HoverStationaryDelay;
    float HoverDelayShort;
    float HoverDelayNormal;
    ImGuiHoveredFlags HoverFlagsForTooltipMouse;
    ImGuiHoveredFlags HoverFlagsForTooltipNav;
};
struct ImGuiKeyData
{
   _Bool         Down;
    float DownDuration;
    float DownDurationPrev;
    float AnalogValue;
};
typedef struct ImVector_ImWchar {int Size;int Capacity;ImWchar* Data;} ImVector_ImWchar;
struct ImGuiIO
{
    ImGuiConfigFlags ConfigFlags;
    ImGuiBackendFlags BackendFlags;
    ImVec2 DisplaySize;
    float DeltaTime;
    float IniSavingRate;
    const char* IniFilename;
    const char* LogFilename;
    void* UserData;
    ImFontAtlas*Fonts;
    float FontGlobalScale;
   _Bool         FontAllowUserScaling;
    ImFont* FontDefault;
    ImVec2 DisplayFramebufferScale;
   _Bool         ConfigNavSwapGamepadButtons;
   _Bool         ConfigNavMoveSetMousePos;
   _Bool         ConfigNavCaptureKeyboard;
   _Bool         ConfigNavEscapeClearFocusItem;
   _Bool         ConfigNavEscapeClearFocusWindow;
   _Bool         ConfigNavCursorVisibleAuto;
   _Bool         ConfigNavCursorVisibleAlways;
   _Bool         ConfigDockingNoSplit;
   _Bool         ConfigDockingWithShift;
   _Bool         ConfigDockingAlwaysTabBar;
   _Bool         ConfigDockingTransparentPayload;
   _Bool         ConfigViewportsNoAutoMerge;
   _Bool         ConfigViewportsNoTaskBarIcon;
   _Bool         ConfigViewportsNoDecoration;
   _Bool         ConfigViewportsNoDefaultParent;
   _Bool         MouseDrawCursor;
   _Bool         ConfigMacOSXBehaviors;
   _Bool         ConfigInputTrickleEventQueue;
   _Bool         ConfigInputTextCursorBlink;
   _Bool         ConfigInputTextEnterKeepActive;
   _Bool         ConfigDragClickToInputText;
   _Bool         ConfigWindowsResizeFromEdges;
   _Bool         ConfigWindowsMoveFromTitleBarOnly;
   _Bool         ConfigScrollbarScrollByPage;
    float ConfigMemoryCompactTimer;
    float MouseDoubleClickTime;
    float MouseDoubleClickMaxDist;
    float MouseDragThreshold;
    float KeyRepeatDelay;
    float KeyRepeatRate;
   _Bool         ConfigErrorRecovery;
   _Bool         ConfigErrorRecoveryEnableAssert;
   _Bool         ConfigErrorRecoveryEnableDebugLog;
   _Bool         ConfigErrorRecoveryEnableTooltip;
   _Bool         ConfigDebugIsDebuggerPresent;
   _Bool         ConfigDebugHighlightIdConflicts;
   _Bool         ConfigDebugBeginReturnValueOnce;
   _Bool         ConfigDebugBeginReturnValueLoop;
   _Bool         ConfigDebugIgnoreFocusLoss;
   _Bool         ConfigDebugIniSettings;
    const char* BackendPlatformName;
    const char* BackendRendererName;
    void* BackendPlatformUserData;
    void* BackendRendererUserData;
    void* BackendLanguageUserData;
   _Bool         WantCaptureMouse;
   _Bool         WantCaptureKeyboard;
   _Bool         WantTextInput;
   _Bool         WantSetMousePos;
   _Bool         WantSaveIniSettings;
   _Bool         NavActive;
   _Bool         NavVisible;
    float Framerate;
    int MetricsRenderVertices;
    int MetricsRenderIndices;
    int MetricsRenderWindows;
    int MetricsActiveWindows;
    ImVec2 MouseDelta;
    ImGuiContext* Ctx;
    ImVec2 MousePos;
   _Bool         MouseDown[5];
    float MouseWheel;
    float MouseWheelH;
    ImGuiMouseSource MouseSource;
    ImGuiID MouseHoveredViewport;
   _Bool         KeyCtrl;
   _Bool         KeyShift;
   _Bool         KeyAlt;
   _Bool         KeySuper;
    ImGuiKeyChord KeyMods;
    ImGuiKeyData KeysData[ImGuiKey_KeysData_SIZE];
   _Bool         WantCaptureMouseUnlessPopupClose;
    ImVec2 MousePosPrev;
    ImVec2 MouseClickedPos[5];
    double MouseClickedTime[5];
   _Bool         MouseClicked[5];
   _Bool         MouseDoubleClicked[5];
    ImU16 MouseClickedCount[5];
    ImU16 MouseClickedLastCount[5];
   _Bool         MouseReleased[5];
   _Bool         MouseDownOwned[5];
   _Bool         MouseDownOwnedUnlessPopupClose[5];
   _Bool         MouseWheelRequestAxisSwap;
   _Bool         MouseCtrlLeftAsRightClick;
    float MouseDownDuration[5];
    float MouseDownDurationPrev[5];
    ImVec2 MouseDragMaxDistanceAbs[5];
    float MouseDragMaxDistanceSqr[5];
    float PenPressure;
   _Bool         AppFocusLost;
   _Bool         AppAcceptingEvents;
    ImS8 BackendUsingLegacyKeyArrays;
   _Bool         BackendUsingLegacyNavInputArray;
    ImWchar16 InputQueueSurrogate;
    ImVector_ImWchar InputQueueCharacters;
};
struct ImGuiInputTextCallbackData
{
    ImGuiContext* Ctx;
    ImGuiInputTextFlags EventFlag;
    ImGuiInputTextFlags Flags;
    void* UserData;
    ImWchar EventChar;
    ImGuiKey EventKey;
    char* Buf;
    int BufTextLen;
    int BufSize;
   _Bool         BufDirty;
    int CursorPos;
    int SelectionStart;
    int SelectionEnd;
};
struct ImGuiSizeCallbackData
{
    void* UserData;
    ImVec2 Pos;
    ImVec2 CurrentSize;
    ImVec2 DesiredSize;
};
struct ImGuiWindowClass
{
    ImGuiID ClassId;
    ImGuiID ParentViewportId;
    ImGuiID FocusRouteParentWindowId;
    ImGuiViewportFlags ViewportFlagsOverrideSet;
    ImGuiViewportFlags ViewportFlagsOverrideClear;
    ImGuiTabItemFlags TabItemFlagsOverrideSet;
    ImGuiDockNodeFlags DockNodeFlagsOverrideSet;
   _Bool         DockingAlwaysTabBar;
   _Bool         DockingAllowUnclassed;
};
struct ImGuiPayload
{
    void* Data;
    int DataSize;
    ImGuiID SourceId;
    ImGuiID SourceParentId;
    int DataFrameCount;
    char DataType[32 + 1];
   _Bool         Preview;
   _Bool         Delivery;
};
struct ImGuiOnceUponAFrame
{
     int RefFrame;
};
struct ImGuiTextRange
{
        const char* b;
        const char* e;
};
typedef struct ImGuiTextRange ImGuiTextRange;
typedef struct ImVector_ImGuiTextRange {int Size;int Capacity;ImGuiTextRange* Data;} ImVector_ImGuiTextRange;
struct ImGuiTextFilter
{
    char InputBuf[256];
    ImVector_ImGuiTextRange Filters;
    int CountGrep;
};
typedef struct ImGuiTextRange ImGuiTextRange;
typedef struct ImVector_char {int Size;int Capacity;char* Data;} ImVector_char;
struct ImGuiTextBuffer
{
    ImVector_char Buf;
};
struct ImGuiStoragePair
{
    ImGuiID key;
    union { int val_i; float val_f; void* val_p; };
};
typedef struct ImVector_ImGuiStoragePair {int Size;int Capacity;ImGuiStoragePair* Data;} ImVector_ImGuiStoragePair;
struct ImGuiStorage
{
    ImVector_ImGuiStoragePair Data;
};
struct ImGuiListClipper
{
    ImGuiContext* Ctx;
    int DisplayStart;
    int DisplayEnd;
    int ItemsCount;
    float ItemsHeight;
    float StartPosY;
    double StartSeekOffsetY;
    void* TempData;
};
struct ImColor
{
    ImVec4 Value;
};
typedef enum {
    ImGuiMultiSelectFlags_None = 0,
    ImGuiMultiSelectFlags_SingleSelect = 1 << 0,
    ImGuiMultiSelectFlags_NoSelectAll = 1 << 1,
    ImGuiMultiSelectFlags_NoRangeSelect = 1 << 2,
    ImGuiMultiSelectFlags_NoAutoSelect = 1 << 3,
    ImGuiMultiSelectFlags_NoAutoClear = 1 << 4,
    ImGuiMultiSelectFlags_NoAutoClearOnReselect = 1 << 5,
    ImGuiMultiSelectFlags_BoxSelect1d = 1 << 6,
    ImGuiMultiSelectFlags_BoxSelect2d = 1 << 7,
    ImGuiMultiSelectFlags_BoxSelectNoScroll = 1 << 8,
    ImGuiMultiSelectFlags_ClearOnEscape = 1 << 9,
    ImGuiMultiSelectFlags_ClearOnClickVoid = 1 << 10,
    ImGuiMultiSelectFlags_ScopeWindow = 1 << 11,
    ImGuiMultiSelectFlags_ScopeRect = 1 << 12,
    ImGuiMultiSelectFlags_SelectOnClick = 1 << 13,
    ImGuiMultiSelectFlags_SelectOnClickRelease = 1 << 14,
    ImGuiMultiSelectFlags_NavWrapX = 1 << 16,
}ImGuiMultiSelectFlags_;
typedef struct ImVector_ImGuiSelectionRequest {int Size;int Capacity;ImGuiSelectionRequest* Data;} ImVector_ImGuiSelectionRequest;
struct ImGuiMultiSelectIO
{
    ImVector_ImGuiSelectionRequest Requests;
    ImGuiSelectionUserData RangeSrcItem;
    ImGuiSelectionUserData NavIdItem;
   _Bool         NavIdSelected;
   _Bool         RangeSrcReset;
    int ItemsCount;
};
typedef enum {
    ImGuiSelectionRequestType_None = 0,
    ImGuiSelectionRequestType_SetAll,
    ImGuiSelectionRequestType_SetRange,
}ImGuiSelectionRequestType;
struct ImGuiSelectionRequest
{
    ImGuiSelectionRequestType Type;
   _Bool         Selected;
    ImS8 RangeDirection;
    ImGuiSelectionUserData RangeFirstItem;
    ImGuiSelectionUserData RangeLastItem;
};
struct ImGuiSelectionBasicStorage
{
    int Size;
   _Bool         PreserveOrder;
    void* UserData;
    ImGuiID (*AdapterIndexToStorageId)(ImGuiSelectionBasicStorage* self, int idx);
    int _SelectionOrder;
    ImGuiStorage _Storage;
};
struct ImGuiSelectionExternalStorage
{
    void* UserData;
    void (*AdapterSetItemSelected)(ImGuiSelectionExternalStorage* self, int idx,                                                                                 _Bool                                                                                      selected);
};
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
struct ImDrawCmd
{
    ImVec4 ClipRect;
    ImTextureID TextureId;
    unsigned int VtxOffset;
    unsigned int IdxOffset;
    unsigned int ElemCount;
    ImDrawCallback UserCallback;
    void* UserCallbackData;
    int UserCallbackDataSize;
    int UserCallbackDataOffset;
};
struct ImDrawVert
{
    ImVec2 pos;
    ImVec2 uv;
    ImU32 col;
};
typedef struct ImDrawCmdHeader ImDrawCmdHeader;
struct ImDrawCmdHeader
{
    ImVec4 ClipRect;
    ImTextureID TextureId;
    unsigned int VtxOffset;
};
typedef struct ImVector_ImDrawCmd {int Size;int Capacity;ImDrawCmd* Data;} ImVector_ImDrawCmd;
typedef struct ImVector_ImDrawIdx {int Size;int Capacity;ImDrawIdx* Data;} ImVector_ImDrawIdx;
struct ImDrawChannel
{
    ImVector_ImDrawCmd _CmdBuffer;
    ImVector_ImDrawIdx _IdxBuffer;
};
typedef struct ImVector_ImDrawChannel {int Size;int Capacity;ImDrawChannel* Data;} ImVector_ImDrawChannel;
struct ImDrawListSplitter
{
    int _Current;
    int _Count;
    ImVector_ImDrawChannel _Channels;
};
typedef enum {
    ImDrawFlags_None = 0,
    ImDrawFlags_Closed = 1 << 0,
    ImDrawFlags_RoundCornersTopLeft = 1 << 4,
    ImDrawFlags_RoundCornersTopRight = 1 << 5,
    ImDrawFlags_RoundCornersBottomLeft = 1 << 6,
    ImDrawFlags_RoundCornersBottomRight = 1 << 7,
    ImDrawFlags_RoundCornersNone = 1 << 8,
    ImDrawFlags_RoundCornersTop = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersBottom = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersLeft = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft,
    ImDrawFlags_RoundCornersRight = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersAll = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersDefault_ = ImDrawFlags_RoundCornersAll,
    ImDrawFlags_RoundCornersMask_ = ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone,
}ImDrawFlags_;
typedef enum {
    ImDrawListFlags_None = 0,
    ImDrawListFlags_AntiAliasedLines = 1 << 0,
    ImDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,
    ImDrawListFlags_AntiAliasedFill = 1 << 2,
    ImDrawListFlags_AllowVtxOffset = 1 << 3,
}ImDrawListFlags_;
typedef struct ImVector_ImDrawVert {int Size;int Capacity;ImDrawVert* Data;} ImVector_ImDrawVert;
typedef struct ImVector_ImVec2 {int Size;int Capacity;ImVec2* Data;} ImVector_ImVec2;
typedef struct ImVector_ImVec4 {int Size;int Capacity;ImVec4* Data;} ImVector_ImVec4;
typedef struct ImVector_ImTextureID {int Size;int Capacity;ImTextureID* Data;} ImVector_ImTextureID;
typedef struct ImVector_ImU8 {int Size;int Capacity;ImU8* Data;} ImVector_ImU8;
struct ImDrawList
{
    ImVector_ImDrawCmd CmdBuffer;
    ImVector_ImDrawIdx IdxBuffer;
    ImVector_ImDrawVert VtxBuffer;
    ImDrawListFlags Flags;
    unsigned int _VtxCurrentIdx;
    ImDrawListSharedData* _Data;
    ImDrawVert* _VtxWritePtr;
    ImDrawIdx* _IdxWritePtr;
    ImVector_ImVec2 _Path;
    ImDrawCmdHeader _CmdHeader;
    ImDrawListSplitter _Splitter;
    ImVector_ImVec4 _ClipRectStack;
    ImVector_ImTextureID _TextureIdStack;
    ImVector_ImU8 _CallbacksDataBuf;
    float _FringeScale;
    const char* _OwnerName;
};
typedef struct ImVector_ImDrawListPtr {int Size;int Capacity;ImDrawList** Data;} ImVector_ImDrawListPtr;
struct ImDrawData
{
   _Bool         Valid;
    int CmdListsCount;
    int TotalIdxCount;
    int TotalVtxCount;
    ImVector_ImDrawListPtr CmdLists;
    ImVec2 DisplayPos;
    ImVec2 DisplaySize;
    ImVec2 FramebufferScale;
    ImGuiViewport* OwnerViewport;
};
struct ImFontConfig
{
    void* FontData;
    int FontDataSize;
   _Bool         FontDataOwnedByAtlas;
    int FontNo;
    float SizePixels;
    int OversampleH;
    int OversampleV;
   _Bool         PixelSnapH;
    ImVec2 GlyphExtraSpacing;
    ImVec2 GlyphOffset;
    const ImWchar* GlyphRanges;
    float GlyphMinAdvanceX;
    float GlyphMaxAdvanceX;
   _Bool         MergeMode;
    unsigned int FontBuilderFlags;
    float RasterizerMultiply;
    float RasterizerDensity;
    ImWchar EllipsisChar;
    char Name[40];
    ImFont* DstFont;
};
struct ImFontGlyph
{
    unsigned int Colored : 1;
    unsigned int Visible : 1;
    unsigned int Codepoint : 30;
    float AdvanceX;
    float X0, Y0, X1, Y1;
    float U0, V0, U1, V1;
};
typedef struct ImVector_ImU32 {int Size;int Capacity;ImU32* Data;} ImVector_ImU32;
struct ImFontGlyphRangesBuilder
{
    ImVector_ImU32 UsedChars;
};
typedef struct ImFontAtlasCustomRect ImFontAtlasCustomRect;
struct ImFontAtlasCustomRect
{
    unsigned short Width, Height;
    unsigned short X, Y;
    unsigned int GlyphID;
    float GlyphAdvanceX;
    ImVec2 GlyphOffset;
    ImFont* Font;
};
typedef enum {
    ImFontAtlasFlags_None = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,
    ImFontAtlasFlags_NoMouseCursors = 1 << 1,
    ImFontAtlasFlags_NoBakedLines = 1 << 2,
}ImFontAtlasFlags_;
typedef struct ImVector_ImFontPtr {int Size;int Capacity;ImFont** Data;} ImVector_ImFontPtr;
typedef struct ImVector_ImFontAtlasCustomRect {int Size;int Capacity;ImFontAtlasCustomRect* Data;} ImVector_ImFontAtlasCustomRect;
typedef struct ImVector_ImFontConfig {int Size;int Capacity;ImFontConfig* Data;} ImVector_ImFontConfig;
struct ImFontAtlas
{
    ImFontAtlasFlags Flags;
    ImTextureID TexID;
    int TexDesiredWidth;
    int TexGlyphPadding;
   _Bool         Locked;
    void* UserData;
   _Bool         TexReady;
   _Bool         TexPixelsUseColors;
    unsigned char* TexPixelsAlpha8;
    unsigned int* TexPixelsRGBA32;
    int TexWidth;
    int TexHeight;
    ImVec2 TexUvScale;
    ImVec2 TexUvWhitePixel;
    ImVector_ImFontPtr Fonts;
    ImVector_ImFontAtlasCustomRect CustomRects;
    ImVector_ImFontConfig ConfigData;
    ImVec4 TexUvLines[(63) + 1];
    const ImFontBuilderIO* FontBuilderIO;
    unsigned int FontBuilderFlags;
    int PackIdMouseCursors;
    int PackIdLines;
};
typedef struct ImVector_float {int Size;int Capacity;float* Data;} ImVector_float;
typedef struct ImVector_ImFontGlyph {int Size;int Capacity;ImFontGlyph* Data;} ImVector_ImFontGlyph;
struct ImFont
{
    ImVector_float IndexAdvanceX;
    float FallbackAdvanceX;
    float FontSize;
    ImVector_ImWchar IndexLookup;
    ImVector_ImFontGlyph Glyphs;
    const ImFontGlyph* FallbackGlyph;
    ImFontAtlas* ContainerAtlas;
    const ImFontConfig* ConfigData;
    short ConfigDataCount;
    ImWchar FallbackChar;
    ImWchar EllipsisChar;
    short EllipsisCharCount;
    float EllipsisWidth;
    float EllipsisCharStep;
   _Bool         DirtyLookupTables;
    float Scale;
    float Ascent, Descent;
    int MetricsTotalSurface;
    ImU8 Used4kPagesMap[(0xFFFF +1)/4096/8];
};
typedef enum {
    ImGuiViewportFlags_None = 0,
    ImGuiViewportFlags_IsPlatformWindow = 1 << 0,
    ImGuiViewportFlags_IsPlatformMonitor = 1 << 1,
    ImGuiViewportFlags_OwnedByApp = 1 << 2,
    ImGuiViewportFlags_NoDecoration = 1 << 3,
    ImGuiViewportFlags_NoTaskBarIcon = 1 << 4,
    ImGuiViewportFlags_NoFocusOnAppearing = 1 << 5,
    ImGuiViewportFlags_NoFocusOnClick = 1 << 6,
    ImGuiViewportFlags_NoInputs = 1 << 7,
    ImGuiViewportFlags_NoRendererClear = 1 << 8,
    ImGuiViewportFlags_NoAutoMerge = 1 << 9,
    ImGuiViewportFlags_TopMost = 1 << 10,
    ImGuiViewportFlags_CanHostOtherWindows = 1 << 11,
    ImGuiViewportFlags_IsMinimized = 1 << 12,
    ImGuiViewportFlags_IsFocused = 1 << 13,
}ImGuiViewportFlags_;
struct ImGuiViewport
{
    ImGuiID ID;
    ImGuiViewportFlags Flags;
    ImVec2 Pos;
    ImVec2 Size;
    ImVec2 WorkPos;
    ImVec2 WorkSize;
    float DpiScale;
    ImGuiID ParentViewportId;
    ImDrawData* DrawData;
    void* RendererUserData;
    void* PlatformUserData;
    void* PlatformHandle;
    void* PlatformHandleRaw;
   _Bool         PlatformWindowCreated;
   _Bool         PlatformRequestMove;
   _Bool         PlatformRequestResize;
   _Bool         PlatformRequestClose;
};
typedef struct ImVector_ImGuiPlatformMonitor {int Size;int Capacity;ImGuiPlatformMonitor* Data;} ImVector_ImGuiPlatformMonitor;
typedef struct ImVector_ImGuiViewportPtr {int Size;int Capacity;ImGuiViewport** Data;} ImVector_ImGuiViewportPtr;
struct ImGuiPlatformIO
{
    const char* (*Platform_GetClipboardTextFn)(ImGuiContext* ctx);
    void (*Platform_SetClipboardTextFn)(ImGuiContext* ctx, const char* text);
    void* Platform_ClipboardUserData;
   _Bool         (*Platform_OpenInShellFn)(ImGuiContext* ctx, const char* path);
    void* Platform_OpenInShellUserData;
    void (*Platform_SetImeDataFn)(ImGuiContext* ctx, ImGuiViewport* viewport, ImGuiPlatformImeData* data);
    void* Platform_ImeUserData;
    ImWchar Platform_LocaleDecimalPoint;
    void* Renderer_RenderState;
    void (*Platform_CreateWindow)(ImGuiViewport* vp);
    void (*Platform_DestroyWindow)(ImGuiViewport* vp);
    void (*Platform_ShowWindow)(ImGuiViewport* vp);
    void (*Platform_SetWindowPos)(ImGuiViewport* vp, ImVec2 pos);
    ImVec2 (*Platform_GetWindowPos)(ImGuiViewport* vp);
    void (*Platform_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);
    ImVec2 (*Platform_GetWindowSize)(ImGuiViewport* vp);
    void (*Platform_SetWindowFocus)(ImGuiViewport* vp);
   _Bool         (*Platform_GetWindowFocus)(ImGuiViewport* vp);
   _Bool         (*Platform_GetWindowMinimized)(ImGuiViewport* vp);
    void (*Platform_SetWindowTitle)(ImGuiViewport* vp, const char* str);
    void (*Platform_SetWindowAlpha)(ImGuiViewport* vp, float alpha);
    void (*Platform_UpdateWindow)(ImGuiViewport* vp);
    void (*Platform_RenderWindow)(ImGuiViewport* vp, void* render_arg);
    void (*Platform_SwapBuffers)(ImGuiViewport* vp, void* render_arg);
    float (*Platform_GetWindowDpiScale)(ImGuiViewport* vp);
    void (*Platform_OnChangedViewport)(ImGuiViewport* vp);
    ImVec4 (*Platform_GetWindowWorkAreaInsets)(ImGuiViewport* vp);
    int (*Platform_CreateVkSurface)(ImGuiViewport* vp, ImU64 vk_inst, const void* vk_allocators, ImU64* out_vk_surface);
    void (*Renderer_CreateWindow)(ImGuiViewport* vp);
    void (*Renderer_DestroyWindow)(ImGuiViewport* vp);
    void (*Renderer_SetWindowSize)(ImGuiViewport* vp, ImVec2 size);
    void (*Renderer_RenderWindow)(ImGuiViewport* vp, void* render_arg);
    void (*Renderer_SwapBuffers)(ImGuiViewport* vp, void* render_arg);
    ImVector_ImGuiPlatformMonitor Monitors;
    ImVector_ImGuiViewportPtr Viewports;
};
struct ImGuiPlatformMonitor
{
    ImVec2 MainPos, MainSize;
    ImVec2 WorkPos, WorkSize;
    float DpiScale;
    void* PlatformHandle;
};
struct ImGuiPlatformImeData
{
   _Bool         WantVisible;
    ImVec2 InputPos;
    float InputLineHeight;
};
struct ImBitVector;
struct ImRect;
struct ImDrawDataBuilder;
struct ImDrawListSharedData;
struct ImGuiBoxSelectState;
struct ImGuiColorMod;
struct ImGuiContext;
struct ImGuiContextHook;
struct ImGuiDataVarInfo;
struct ImGuiDataTypeInfo;
struct ImGuiDockContext;
struct ImGuiDockRequest;
struct ImGuiDockNode;
struct ImGuiDockNodeSettings;
struct ImGuiErrorRecoveryState;
struct ImGuiGroupData;
struct ImGuiInputTextState;
struct ImGuiInputTextDeactivateData;
struct ImGuiLastItemData;
struct ImGuiLocEntry;
struct ImGuiMenuColumns;
struct ImGuiMultiSelectState;
struct ImGuiMultiSelectTempData;
struct ImGuiNavItemData;
struct ImGuiMetricsConfig;
struct ImGuiNextWindowData;
struct ImGuiNextItemData;
struct ImGuiOldColumnData;
struct ImGuiOldColumns;
struct ImGuiPopupData;
struct ImGuiSettingsHandler;
struct ImGuiStyleMod;
struct ImGuiTabBar;
struct ImGuiTabItem;
struct ImGuiTable;
struct ImGuiTableHeaderData;
struct ImGuiTableColumn;
struct ImGuiTableInstanceData;
struct ImGuiTableTempData;
struct ImGuiTableSettings;
struct ImGuiTableColumnsSettings;
struct ImGuiTreeNodeStackData;
struct ImGuiTypingSelectState;
struct ImGuiTypingSelectRequest;
struct ImGuiWindow;
struct ImGuiWindowDockStyle;
struct ImGuiWindowTempData;
struct ImGuiWindowSettings;
typedef int ImGuiDataAuthority;
typedef int ImGuiLayoutType;
typedef int ImGuiActivateFlags;
typedef int ImGuiDebugLogFlags;
typedef int ImGuiFocusRequestFlags;
typedef int ImGuiItemStatusFlags;
typedef int ImGuiOldColumnFlags;
typedef int ImGuiNavRenderCursorFlags;
typedef int ImGuiNavMoveFlags;
typedef int ImGuiNextItemDataFlags;
typedef int ImGuiNextWindowDataFlags;
typedef int ImGuiScrollFlags;
typedef int ImGuiSeparatorFlags;
typedef int ImGuiTextFlags;
typedef int ImGuiTooltipFlags;
typedef int ImGuiTypingSelectFlags;
typedef int ImGuiWindowRefreshFlags;
extern ImGuiContext* GImGui;
typedef FILE* ImFileHandle;
typedef struct ImVec1 ImVec1;
struct ImVec1
{
    float x;
};
typedef struct ImVec2ih ImVec2ih;
struct ImVec2ih
{
    short x, y;
};
struct ImRect
{
    ImVec2 Min;
    ImVec2 Max;
};
typedef ImU32* ImBitArrayPtr;
struct ImBitVector
{
    ImVector_ImU32 Storage;
};
typedef int ImPoolIdx;
typedef struct ImGuiTextIndex ImGuiTextIndex;
typedef struct ImVector_int {int Size;int Capacity;int* Data;} ImVector_int;
struct ImGuiTextIndex
{
    ImVector_int LineOffsets;
    int EndOffset;
};
struct ImDrawListSharedData
{
    ImVec2 TexUvWhitePixel;
    ImFont* Font;
    float FontSize;
    float FontScale;
    float CurveTessellationTol;
    float CircleSegmentMaxError;
    ImVec4 ClipRectFullscreen;
    ImDrawListFlags InitialFlags;
    ImVector_ImVec2 TempBuffer;
    ImVec2 ArcFastVtx[48];
    float ArcFastRadiusCutoff;
    ImU8 CircleSegmentCounts[64];
    const ImVec4* TexUvLines;
};
struct ImDrawDataBuilder
{
    ImVector_ImDrawListPtr* Layers[2];
    ImVector_ImDrawListPtr LayerData1;
};
struct ImGuiDataVarInfo
{
    ImGuiDataType Type;
    ImU32 Count;
    ImU32 Offset;
};
typedef struct ImGuiDataTypeStorage ImGuiDataTypeStorage;
struct ImGuiDataTypeStorage
{
    ImU8 Data[8];
};
struct ImGuiDataTypeInfo
{
    size_t Size;
    const char* Name;
    const char* PrintFmt;
    const char* ScanFmt;
};
typedef enum {
    ImGuiDataType_String = ImGuiDataType_COUNT + 1,
    ImGuiDataType_Pointer,
    ImGuiDataType_ID,
}ImGuiDataTypePrivate_;
typedef enum {
    ImGuiItemFlags_Disabled = 1 << 10,
    ImGuiItemFlags_ReadOnly = 1 << 11,
    ImGuiItemFlags_MixedValue = 1 << 12,
    ImGuiItemFlags_NoWindowHoverableCheck = 1 << 13,
    ImGuiItemFlags_AllowOverlap = 1 << 14,
    ImGuiItemFlags_NoNavDisableMouseHover = 1 << 15,
    ImGuiItemFlags_NoMarkEdited = 1 << 16,
    ImGuiItemFlags_Inputable = 1 << 20,
    ImGuiItemFlags_HasSelectionUserData = 1 << 21,
    ImGuiItemFlags_IsMultiSelect = 1 << 22,
    ImGuiItemFlags_Default_ = ImGuiItemFlags_AutoClosePopups,
}ImGuiItemFlagsPrivate_;
typedef enum {
    ImGuiItemStatusFlags_None = 0,
    ImGuiItemStatusFlags_HoveredRect = 1 << 0,
    ImGuiItemStatusFlags_HasDisplayRect = 1 << 1,
    ImGuiItemStatusFlags_Edited = 1 << 2,
    ImGuiItemStatusFlags_ToggledSelection = 1 << 3,
    ImGuiItemStatusFlags_ToggledOpen = 1 << 4,
    ImGuiItemStatusFlags_HasDeactivated = 1 << 5,
    ImGuiItemStatusFlags_Deactivated = 1 << 6,
    ImGuiItemStatusFlags_HoveredWindow = 1 << 7,
    ImGuiItemStatusFlags_Visible = 1 << 8,
    ImGuiItemStatusFlags_HasClipRect = 1 << 9,
    ImGuiItemStatusFlags_HasShortcut = 1 << 10,
}ImGuiItemStatusFlags_;
typedef enum {
    ImGuiHoveredFlags_DelayMask_ = ImGuiHoveredFlags_DelayNone | ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay,
    ImGuiHoveredFlags_AllowedMaskForIsWindowHovered = ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_NoPopupHierarchy | ImGuiHoveredFlags_DockHierarchy | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary,
    ImGuiHoveredFlags_AllowedMaskForIsItemHovered = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped | ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_NoNavOverride | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayMask_,
}ImGuiHoveredFlagsPrivate_;
typedef enum {
    ImGuiInputTextFlags_Multiline = 1 << 26,
    ImGuiInputTextFlags_MergedItem = 1 << 27,
    ImGuiInputTextFlags_LocalizeDecimalPoint= 1 << 28,
}ImGuiInputTextFlagsPrivate_;
typedef enum {
    ImGuiButtonFlags_PressedOnClick = 1 << 4,
    ImGuiButtonFlags_PressedOnClickRelease = 1 << 5,
    ImGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6,
    ImGuiButtonFlags_PressedOnRelease = 1 << 7,
    ImGuiButtonFlags_PressedOnDoubleClick = 1 << 8,
    ImGuiButtonFlags_PressedOnDragDropHold = 1 << 9,
    ImGuiButtonFlags_FlattenChildren = 1 << 11,
    ImGuiButtonFlags_AllowOverlap = 1 << 12,
    ImGuiButtonFlags_AlignTextBaseLine = 1 << 15,
    ImGuiButtonFlags_NoKeyModsAllowed = 1 << 16,
    ImGuiButtonFlags_NoHoldingActiveId = 1 << 17,
    ImGuiButtonFlags_NoNavFocus = 1 << 18,
    ImGuiButtonFlags_NoHoveredOnFocus = 1 << 19,
    ImGuiButtonFlags_NoSetKeyOwner = 1 << 20,
    ImGuiButtonFlags_NoTestKeyOwner = 1 << 21,
    ImGuiButtonFlags_PressedOnMask_ = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClickReleaseAnywhere | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick | ImGuiButtonFlags_PressedOnDragDropHold,
    ImGuiButtonFlags_PressedOnDefault_ = ImGuiButtonFlags_PressedOnClickRelease,
}ImGuiButtonFlagsPrivate_;
typedef enum {
    ImGuiComboFlags_CustomPreview = 1 << 20,
}ImGuiComboFlagsPrivate_;
typedef enum {
    ImGuiSliderFlags_Vertical = 1 << 20,
    ImGuiSliderFlags_ReadOnly = 1 << 21,
}ImGuiSliderFlagsPrivate_;
typedef enum {
    ImGuiSelectableFlags_NoHoldingActiveID = 1 << 20,
    ImGuiSelectableFlags_SelectOnNav = 1 << 21,
    ImGuiSelectableFlags_SelectOnClick = 1 << 22,
    ImGuiSelectableFlags_SelectOnRelease = 1 << 23,
    ImGuiSelectableFlags_SpanAvailWidth = 1 << 24,
    ImGuiSelectableFlags_SetNavIdOnHover = 1 << 25,
    ImGuiSelectableFlags_NoPadWithHalfSpacing = 1 << 26,
    ImGuiSelectableFlags_NoSetKeyOwner = 1 << 27,
}ImGuiSelectableFlagsPrivate_;
typedef enum {
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 28,
    ImGuiTreeNodeFlags_UpsideDownArrow = 1 << 29,
    ImGuiTreeNodeFlags_OpenOnMask_ = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow,
}ImGuiTreeNodeFlagsPrivate_;
typedef enum {
    ImGuiSeparatorFlags_None = 0,
    ImGuiSeparatorFlags_Horizontal = 1 << 0,
    ImGuiSeparatorFlags_Vertical = 1 << 1,
    ImGuiSeparatorFlags_SpanAllColumns = 1 << 2,
}ImGuiSeparatorFlags_;
typedef enum {
    ImGuiFocusRequestFlags_None = 0,
    ImGuiFocusRequestFlags_RestoreFocusedChild = 1 << 0,
    ImGuiFocusRequestFlags_UnlessBelowModal = 1 << 1,
}ImGuiFocusRequestFlags_;
typedef enum {
    ImGuiTextFlags_None = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0,
}ImGuiTextFlags_;
typedef enum {
    ImGuiTooltipFlags_None = 0,
    ImGuiTooltipFlags_OverridePrevious = 1 << 1,
}ImGuiTooltipFlags_;
typedef enum {
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical = 1
}ImGuiLayoutType_;
typedef enum {
    ImGuiLogType_None = 0,
    ImGuiLogType_TTY,
    ImGuiLogType_File,
    ImGuiLogType_Buffer,
    ImGuiLogType_Clipboard,
}ImGuiLogType;
typedef enum {
    ImGuiAxis_None = -1,
    ImGuiAxis_X = 0,
    ImGuiAxis_Y = 1
}ImGuiAxis;
typedef enum {
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram,
}ImGuiPlotType;
struct ImGuiColorMod
{
    ImGuiCol Col;
    ImVec4 BackupValue;
};
struct ImGuiStyleMod
{
    ImGuiStyleVar VarIdx;
    union { int BackupInt[2]; float BackupFloat[2]; };
};
typedef struct ImGuiComboPreviewData ImGuiComboPreviewData;
struct ImGuiComboPreviewData
{
    ImRect PreviewRect;
    ImVec2 BackupCursorPos;
    ImVec2 BackupCursorMaxPos;
    ImVec2 BackupCursorPosPrevLine;
    float BackupPrevLineTextBaseOffset;
    ImGuiLayoutType BackupLayout;
};
struct ImGuiGroupData
{
    ImGuiID WindowID;
    ImVec2 BackupCursorPos;
    ImVec2 BackupCursorMaxPos;
    ImVec2 BackupCursorPosPrevLine;
    ImVec1 BackupIndent;
    ImVec1 BackupGroupOffset;
    ImVec2 BackupCurrLineSize;
    float BackupCurrLineTextBaseOffset;
    ImGuiID BackupActiveIdIsAlive;
   _Bool         BackupActiveIdPreviousFrameIsAlive;
   _Bool         BackupHoveredIdIsAlive;
   _Bool         BackupIsSameLine;
   _Bool         EmitItem;
};
struct ImGuiMenuColumns
{
    ImU32 TotalWidth;
    ImU32 NextTotalWidth;
    ImU16 Spacing;
    ImU16 OffsetIcon;
    ImU16 OffsetLabel;
    ImU16 OffsetShortcut;
    ImU16 OffsetMark;
    ImU16 Widths[4];
};
typedef struct ImGuiInputTextDeactivatedState ImGuiInputTextDeactivatedState;
struct ImGuiInputTextDeactivatedState
{
    ImGuiID ID;
    ImVector_char TextA;
}; struct STB_TexteditState;
typedef STB_TexteditState ImStbTexteditState;
struct ImGuiInputTextState
{
    ImGuiContext* Ctx;
    ImStbTexteditState* Stb;
    ImGuiID ID;
    int CurLenA;
    ImVector_char TextA;
    ImVector_char InitialTextA;
    ImVector_char CallbackTextBackup;
    int BufCapacityA;
    ImVec2 Scroll;
    float CursorAnim;
   _Bool         CursorFollow;
   _Bool         SelectedAllMouseLock;
   _Bool         Edited;
    ImGuiInputTextFlags Flags;
   _Bool         ReloadUserBuf;
    int ReloadSelectionStart;
    int ReloadSelectionEnd;
};
typedef enum {
    ImGuiWindowRefreshFlags_None = 0,
    ImGuiWindowRefreshFlags_TryToAvoidRefresh = 1 << 0,
    ImGuiWindowRefreshFlags_RefreshOnHover = 1 << 1,
    ImGuiWindowRefreshFlags_RefreshOnFocus = 1 << 2,
}ImGuiWindowRefreshFlags_;
typedef enum {
    ImGuiNextWindowDataFlags_None = 0,
    ImGuiNextWindowDataFlags_HasPos = 1 << 0,
    ImGuiNextWindowDataFlags_HasSize = 1 << 1,
    ImGuiNextWindowDataFlags_HasContentSize = 1 << 2,
    ImGuiNextWindowDataFlags_HasCollapsed = 1 << 3,
    ImGuiNextWindowDataFlags_HasSizeConstraint = 1 << 4,
    ImGuiNextWindowDataFlags_HasFocus = 1 << 5,
    ImGuiNextWindowDataFlags_HasBgAlpha = 1 << 6,
    ImGuiNextWindowDataFlags_HasScroll = 1 << 7,
    ImGuiNextWindowDataFlags_HasChildFlags = 1 << 8,
    ImGuiNextWindowDataFlags_HasRefreshPolicy = 1 << 9,
    ImGuiNextWindowDataFlags_HasViewport = 1 << 10,
    ImGuiNextWindowDataFlags_HasDock = 1 << 11,
    ImGuiNextWindowDataFlags_HasWindowClass = 1 << 12,
}ImGuiNextWindowDataFlags_;
struct ImGuiNextWindowData
{
    ImGuiNextWindowDataFlags Flags;
    ImGuiCond PosCond;
    ImGuiCond SizeCond;
    ImGuiCond CollapsedCond;
    ImGuiCond DockCond;
    ImVec2 PosVal;
    ImVec2 PosPivotVal;
    ImVec2 SizeVal;
    ImVec2 ContentSizeVal;
    ImVec2 ScrollVal;
    ImGuiChildFlags ChildFlags;
   _Bool         PosUndock;
   _Bool         CollapsedVal;
    ImRect SizeConstraintRect;
    ImGuiSizeCallback SizeCallback;
    void* SizeCallbackUserData;
    float BgAlphaVal;
    ImGuiID ViewportId;
    ImGuiID DockId;
    ImGuiWindowClass WindowClass;
    ImVec2 MenuBarOffsetMinVal;
    ImGuiWindowRefreshFlags RefreshFlagsVal;
};
typedef enum {
    ImGuiNextItemDataFlags_None = 0,
    ImGuiNextItemDataFlags_HasWidth = 1 << 0,
    ImGuiNextItemDataFlags_HasOpen = 1 << 1,
    ImGuiNextItemDataFlags_HasShortcut = 1 << 2,
    ImGuiNextItemDataFlags_HasRefVal = 1 << 3,
    ImGuiNextItemDataFlags_HasStorageID = 1 << 4,
}ImGuiNextItemDataFlags_;
struct ImGuiNextItemData
{
    ImGuiNextItemDataFlags HasFlags;
    ImGuiItemFlags ItemFlags;
    ImGuiID FocusScopeId;
    ImGuiSelectionUserData SelectionUserData;
    float Width;
    ImGuiKeyChord Shortcut;
    ImGuiInputFlags ShortcutFlags;
   _Bool         OpenVal;
    ImU8 OpenCond;
    ImGuiDataTypeStorage RefVal;
    ImGuiID StorageId;
};
struct ImGuiLastItemData
{
    ImGuiID ID;
    ImGuiItemFlags ItemFlags;
    ImGuiItemStatusFlags StatusFlags;
    ImRect Rect;
    ImRect NavRect;
    ImRect DisplayRect;
    ImRect ClipRect;
    ImGuiKeyChord Shortcut;
};
struct ImGuiTreeNodeStackData
{
    ImGuiID ID;
    ImGuiTreeNodeFlags TreeFlags;
    ImGuiItemFlags ItemFlags;
    ImRect NavRect;
};
struct ImGuiErrorRecoveryState
{
    short SizeOfWindowStack;
    short SizeOfIDStack;
    short SizeOfTreeStack;
    short SizeOfColorStack;
    short SizeOfStyleVarStack;
    short SizeOfFontStack;
    short SizeOfFocusScopeStack;
    short SizeOfGroupStack;
    short SizeOfItemFlagsStack;
    short SizeOfBeginPopupStack;
    short SizeOfDisabledStack;
};
typedef struct ImGuiWindowStackData ImGuiWindowStackData;
struct ImGuiWindowStackData
{
    ImGuiWindow* Window;
    ImGuiLastItemData ParentLastItemDataBackup;
    ImGuiErrorRecoveryState StackSizesInBegin;
   _Bool         DisabledOverrideReenable;
};
typedef struct ImGuiShrinkWidthItem ImGuiShrinkWidthItem;
struct ImGuiShrinkWidthItem
{
    int Index;
    float Width;
    float InitialWidth;
};
typedef struct ImGuiPtrOrIndex ImGuiPtrOrIndex;
struct ImGuiPtrOrIndex
{
    void* Ptr;
    int Index;
};
typedef enum {
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox,
    ImGuiPopupPositionPolicy_Tooltip,
}ImGuiPopupPositionPolicy;
struct ImGuiPopupData
{
    ImGuiID PopupId;
    ImGuiWindow* Window;
    ImGuiWindow* RestoreNavWindow;
    int ParentNavLayer;
    int OpenFrameCount;
    ImGuiID OpenParentId;
    ImVec2 OpenPopupPos;
    ImVec2 OpenMousePos;
};
typedef struct ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN {ImU32 Storage[(ImGuiKey_NamedKey_COUNT+31)>>5];} ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN;
typedef ImBitArray_ImGuiKey_NamedKey_COUNT__lessImGuiKey_NamedKey_BEGIN ImBitArrayForNamedKeys;
typedef enum {
    ImGuiInputEventType_None = 0,
    ImGuiInputEventType_MousePos,
    ImGuiInputEventType_MouseWheel,
    ImGuiInputEventType_MouseButton,
    ImGuiInputEventType_MouseViewport,
    ImGuiInputEventType_Key,
    ImGuiInputEventType_Text,
    ImGuiInputEventType_Focus,
    ImGuiInputEventType_COUNT
}ImGuiInputEventType;
typedef enum {
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Keyboard,
    ImGuiInputSource_Gamepad,
    ImGuiInputSource_COUNT
}ImGuiInputSource;
typedef struct ImGuiInputEventMousePos ImGuiInputEventMousePos;
struct ImGuiInputEventMousePos
{ float PosX, PosY; ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseWheel ImGuiInputEventMouseWheel;
struct ImGuiInputEventMouseWheel
{ float WheelX, WheelY; ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseButton ImGuiInputEventMouseButton;
struct ImGuiInputEventMouseButton
{ int Button;              _Bool                   Down; ImGuiMouseSource MouseSource;
};
typedef struct ImGuiInputEventMouseViewport ImGuiInputEventMouseViewport;
struct ImGuiInputEventMouseViewport
{ ImGuiID HoveredViewportID;
};
typedef struct ImGuiInputEventKey ImGuiInputEventKey;
struct ImGuiInputEventKey
{ ImGuiKey Key;                _Bool                     Down; float AnalogValue;
};
typedef struct ImGuiInputEventText ImGuiInputEventText;
struct ImGuiInputEventText
{ unsigned int Char;
};
typedef struct ImGuiInputEventAppFocused ImGuiInputEventAppFocused;
struct ImGuiInputEventAppFocused
{  _Bool       Focused;
};
typedef struct ImGuiInputEvent ImGuiInputEvent;
struct ImGuiInputEvent
{
    ImGuiInputEventType Type;
    ImGuiInputSource Source;
    ImU32 EventId;
    union
    {
        ImGuiInputEventMousePos MousePos;
        ImGuiInputEventMouseWheel MouseWheel;
        ImGuiInputEventMouseButton MouseButton;
        ImGuiInputEventMouseViewport MouseViewport;
        ImGuiInputEventKey Key;
        ImGuiInputEventText Text;
        ImGuiInputEventAppFocused AppFocused;
    };
   _Bool         AddedByTestEngine;
};
typedef ImS16 ImGuiKeyRoutingIndex;
typedef struct ImGuiKeyRoutingData ImGuiKeyRoutingData;
struct ImGuiKeyRoutingData
{
    ImGuiKeyRoutingIndex NextEntryIndex;
    ImU16 Mods;
    ImU8 RoutingCurrScore;
    ImU8 RoutingNextScore;
    ImGuiID RoutingCurr;
    ImGuiID RoutingNext;
};
typedef struct ImGuiKeyRoutingTable ImGuiKeyRoutingTable;
typedef struct ImVector_ImGuiKeyRoutingData {int Size;int Capacity;ImGuiKeyRoutingData* Data;} ImVector_ImGuiKeyRoutingData;
struct ImGuiKeyRoutingTable
{
    ImGuiKeyRoutingIndex Index[ImGuiKey_NamedKey_COUNT];
    ImVector_ImGuiKeyRoutingData Entries;
    ImVector_ImGuiKeyRoutingData EntriesNext;
};
typedef struct ImGuiKeyOwnerData ImGuiKeyOwnerData;
struct ImGuiKeyOwnerData
{
    ImGuiID OwnerCurr;
    ImGuiID OwnerNext;
   _Bool         LockThisFrame;
   _Bool         LockUntilRelease;
};
typedef enum {
    ImGuiInputFlags_RepeatRateDefault = 1 << 1,
    ImGuiInputFlags_RepeatRateNavMove = 1 << 2,
    ImGuiInputFlags_RepeatRateNavTweak = 1 << 3,
    ImGuiInputFlags_RepeatUntilRelease = 1 << 4,
    ImGuiInputFlags_RepeatUntilKeyModsChange = 1 << 5,
    ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone = 1 << 6,
    ImGuiInputFlags_RepeatUntilOtherKeyPress = 1 << 7,
    ImGuiInputFlags_LockThisFrame = 1 << 20,
    ImGuiInputFlags_LockUntilRelease = 1 << 21,
    ImGuiInputFlags_CondHovered = 1 << 22,
    ImGuiInputFlags_CondActive = 1 << 23,
    ImGuiInputFlags_CondDefault_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
    ImGuiInputFlags_RepeatRateMask_ = ImGuiInputFlags_RepeatRateDefault | ImGuiInputFlags_RepeatRateNavMove | ImGuiInputFlags_RepeatRateNavTweak,
    ImGuiInputFlags_RepeatUntilMask_ = ImGuiInputFlags_RepeatUntilRelease | ImGuiInputFlags_RepeatUntilKeyModsChange | ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone | ImGuiInputFlags_RepeatUntilOtherKeyPress,
    ImGuiInputFlags_RepeatMask_ = ImGuiInputFlags_Repeat | ImGuiInputFlags_RepeatRateMask_ | ImGuiInputFlags_RepeatUntilMask_,
    ImGuiInputFlags_CondMask_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
    ImGuiInputFlags_RouteTypeMask_ = ImGuiInputFlags_RouteActive | ImGuiInputFlags_RouteFocused | ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteAlways,
    ImGuiInputFlags_RouteOptionsMask_ = ImGuiInputFlags_RouteOverFocused | ImGuiInputFlags_RouteOverActive | ImGuiInputFlags_RouteUnlessBgFocused | ImGuiInputFlags_RouteFromRootWindow,
    ImGuiInputFlags_SupportedByIsKeyPressed = ImGuiInputFlags_RepeatMask_,
    ImGuiInputFlags_SupportedByIsMouseClicked = ImGuiInputFlags_Repeat,
    ImGuiInputFlags_SupportedByShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_,
    ImGuiInputFlags_SupportedBySetNextItemShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_ | ImGuiInputFlags_Tooltip,
    ImGuiInputFlags_SupportedBySetKeyOwner = ImGuiInputFlags_LockThisFrame | ImGuiInputFlags_LockUntilRelease,
    ImGuiInputFlags_SupportedBySetItemKeyOwner = ImGuiInputFlags_SupportedBySetKeyOwner | ImGuiInputFlags_CondMask_,
}ImGuiInputFlagsPrivate_;
typedef struct ImGuiListClipperRange ImGuiListClipperRange;
struct ImGuiListClipperRange
{
    int Min;
    int Max;
   _Bool         PosToIndexConvert;
    ImS8 PosToIndexOffsetMin;
    ImS8 PosToIndexOffsetMax;
};
typedef struct ImGuiListClipperData ImGuiListClipperData;
typedef struct ImVector_ImGuiListClipperRange {int Size;int Capacity;ImGuiListClipperRange* Data;} ImVector_ImGuiListClipperRange;
struct ImGuiListClipperData
{
    ImGuiListClipper* ListClipper;
    float LossynessOffset;
    int StepNo;
    int ItemsFrozen;
    ImVector_ImGuiListClipperRange Ranges;
};
typedef enum {
    ImGuiActivateFlags_None = 0,
    ImGuiActivateFlags_PreferInput = 1 << 0,
    ImGuiActivateFlags_PreferTweak = 1 << 1,
    ImGuiActivateFlags_TryToPreserveState = 1 << 2,
    ImGuiActivateFlags_FromTabbing = 1 << 3,
    ImGuiActivateFlags_FromShortcut = 1 << 4,
}ImGuiActivateFlags_;
typedef enum {
    ImGuiScrollFlags_None = 0,
    ImGuiScrollFlags_KeepVisibleEdgeX = 1 << 0,
    ImGuiScrollFlags_KeepVisibleEdgeY = 1 << 1,
    ImGuiScrollFlags_KeepVisibleCenterX = 1 << 2,
    ImGuiScrollFlags_KeepVisibleCenterY = 1 << 3,
    ImGuiScrollFlags_AlwaysCenterX = 1 << 4,
    ImGuiScrollFlags_AlwaysCenterY = 1 << 5,
    ImGuiScrollFlags_NoScrollParent = 1 << 6,
    ImGuiScrollFlags_MaskX_ = ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_KeepVisibleCenterX | ImGuiScrollFlags_AlwaysCenterX,
    ImGuiScrollFlags_MaskY_ = ImGuiScrollFlags_KeepVisibleEdgeY | ImGuiScrollFlags_KeepVisibleCenterY | ImGuiScrollFlags_AlwaysCenterY,
}ImGuiScrollFlags_;
typedef enum {
    ImGuiNavRenderCursorFlags_None = 0,
    ImGuiNavRenderCursorFlags_Compact = 1 << 1,
    ImGuiNavRenderCursorFlags_AlwaysDraw = 1 << 2,
    ImGuiNavRenderCursorFlags_NoRounding = 1 << 3,
}ImGuiNavRenderCursorFlags_;
typedef enum {
    ImGuiNavMoveFlags_None = 0,
    ImGuiNavMoveFlags_LoopX = 1 << 0,
    ImGuiNavMoveFlags_LoopY = 1 << 1,
    ImGuiNavMoveFlags_WrapX = 1 << 2,
    ImGuiNavMoveFlags_WrapY = 1 << 3,
    ImGuiNavMoveFlags_WrapMask_ = ImGuiNavMoveFlags_LoopX | ImGuiNavMoveFlags_LoopY | ImGuiNavMoveFlags_WrapX | ImGuiNavMoveFlags_WrapY,
    ImGuiNavMoveFlags_AllowCurrentNavId = 1 << 4,
    ImGuiNavMoveFlags_AlsoScoreVisibleSet = 1 << 5,
    ImGuiNavMoveFlags_ScrollToEdgeY = 1 << 6,
    ImGuiNavMoveFlags_Forwarded = 1 << 7,
    ImGuiNavMoveFlags_DebugNoResult = 1 << 8,
    ImGuiNavMoveFlags_FocusApi = 1 << 9,
    ImGuiNavMoveFlags_IsTabbing = 1 << 10,
    ImGuiNavMoveFlags_IsPageMove = 1 << 11,
    ImGuiNavMoveFlags_Activate = 1 << 12,
    ImGuiNavMoveFlags_NoSelect = 1 << 13,
    ImGuiNavMoveFlags_NoSetNavCursorVisible = 1 << 14,
    ImGuiNavMoveFlags_NoClearActiveId = 1 << 15,
}ImGuiNavMoveFlags_;
typedef enum {
    ImGuiNavLayer_Main = 0,
    ImGuiNavLayer_Menu = 1,
    ImGuiNavLayer_COUNT
}ImGuiNavLayer;
struct ImGuiNavItemData
{
    ImGuiWindow* Window;
    ImGuiID ID;
    ImGuiID FocusScopeId;
    ImRect RectRel;
    ImGuiItemFlags ItemFlags;
    float DistBox;
    float DistCenter;
    float DistAxial;
    ImGuiSelectionUserData SelectionUserData;
};
typedef struct ImGuiFocusScopeData ImGuiFocusScopeData;
struct ImGuiFocusScopeData
{
    ImGuiID ID;
    ImGuiID WindowID;
};
typedef enum {
    ImGuiTypingSelectFlags_None = 0,
    ImGuiTypingSelectFlags_AllowBackspace = 1 << 0,
    ImGuiTypingSelectFlags_AllowSingleCharMode = 1 << 1,
}ImGuiTypingSelectFlags_;
struct ImGuiTypingSelectRequest
{
    ImGuiTypingSelectFlags Flags;
    int SearchBufferLen;
    const char* SearchBuffer;
   _Bool         SelectRequest;
   _Bool         SingleCharMode;
    ImS8 SingleCharSize;
};
struct ImGuiTypingSelectState
{
    ImGuiTypingSelectRequest Request;
    char SearchBuffer[64];
    ImGuiID FocusScope;
    int LastRequestFrame;
    float LastRequestTime;
   _Bool         SingleCharModeLock;
};
typedef enum {
    ImGuiOldColumnFlags_None = 0,
    ImGuiOldColumnFlags_NoBorder = 1 << 0,
    ImGuiOldColumnFlags_NoResize = 1 << 1,
    ImGuiOldColumnFlags_NoPreserveWidths = 1 << 2,
    ImGuiOldColumnFlags_NoForceWithinWindow = 1 << 3,
    ImGuiOldColumnFlags_GrowParentContentsSize = 1 << 4,
}ImGuiOldColumnFlags_;
struct ImGuiOldColumnData
{
    float OffsetNorm;
    float OffsetNormBeforeResize;
    ImGuiOldColumnFlags Flags;
    ImRect ClipRect;
};
typedef struct ImVector_ImGuiOldColumnData {int Size;int Capacity;ImGuiOldColumnData* Data;} ImVector_ImGuiOldColumnData;
struct ImGuiOldColumns
{
    ImGuiID ID;
    ImGuiOldColumnFlags Flags;
   _Bool         IsFirstFrame;
   _Bool         IsBeingResized;
    int Current;
    int Count;
    float OffMinX, OffMaxX;
    float LineMinY, LineMaxY;
    float HostCursorPosY;
    float HostCursorMaxPosX;
    ImRect HostInitialClipRect;
    ImRect HostBackupClipRect;
    ImRect HostBackupParentWorkRect;
    ImVector_ImGuiOldColumnData Columns;
    ImDrawListSplitter Splitter;
};
struct ImGuiBoxSelectState
{
    ImGuiID ID;
   _Bool         IsActive;
   _Bool         IsStarting;
   _Bool         IsStartedFromVoid;
   _Bool         IsStartedSetNavIdOnce;
   _Bool         RequestClear;
    ImGuiKeyChord KeyMods : 16;
    ImVec2 StartPosRel;
    ImVec2 EndPosRel;
    ImVec2 ScrollAccum;
    ImGuiWindow* Window;
   _Bool         UnclipMode;
    ImRect UnclipRect;
    ImRect BoxSelectRectPrev;
    ImRect BoxSelectRectCurr;
};
struct ImGuiMultiSelectTempData
{
    ImGuiMultiSelectIO IO;
    ImGuiMultiSelectState* Storage;
    ImGuiID FocusScopeId;
    ImGuiMultiSelectFlags Flags;
    ImVec2 ScopeRectMin;
    ImVec2 BackupCursorMaxPos;
    ImGuiSelectionUserData LastSubmittedItem;
    ImGuiID BoxSelectId;
    ImGuiKeyChord KeyMods;
    ImS8 LoopRequestSetAll;
   _Bool         IsEndIO;
   _Bool         IsFocused;
   _Bool         IsKeyboardSetRange;
   _Bool         NavIdPassedBy;
   _Bool         RangeSrcPassedBy;
   _Bool         RangeDstPassedBy;
};
struct ImGuiMultiSelectState
{
    ImGuiWindow* Window;
    ImGuiID ID;
    int LastFrameActive;
    int LastSelectionSize;
    ImS8 RangeSelected;
    ImS8 NavIdSelected;
    ImGuiSelectionUserData RangeSrcItem;
    ImGuiSelectionUserData NavIdItem;
};
typedef enum {
    ImGuiDockNodeFlags_DockSpace = 1 << 10,
    ImGuiDockNodeFlags_CentralNode = 1 << 11,
    ImGuiDockNodeFlags_NoTabBar = 1 << 12,
    ImGuiDockNodeFlags_HiddenTabBar = 1 << 13,
    ImGuiDockNodeFlags_NoWindowMenuButton = 1 << 14,
    ImGuiDockNodeFlags_NoCloseButton = 1 << 15,
    ImGuiDockNodeFlags_NoResizeX = 1 << 16,
    ImGuiDockNodeFlags_NoResizeY = 1 << 17,
    ImGuiDockNodeFlags_DockedWindowsInFocusRoute= 1 << 18,
    ImGuiDockNodeFlags_NoDockingSplitOther = 1 << 19,
    ImGuiDockNodeFlags_NoDockingOverMe = 1 << 20,
    ImGuiDockNodeFlags_NoDockingOverOther = 1 << 21,
    ImGuiDockNodeFlags_NoDockingOverEmpty = 1 << 22,
    ImGuiDockNodeFlags_NoDocking = ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoDockingOverEmpty | ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingSplitOther,
    ImGuiDockNodeFlags_SharedFlagsInheritMask_ = ~0,
    ImGuiDockNodeFlags_NoResizeFlagsMask_ = (int)ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoResizeX | ImGuiDockNodeFlags_NoResizeY,
    ImGuiDockNodeFlags_LocalFlagsTransferMask_ = (int)ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoResizeFlagsMask_ | (int)ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_CentralNode | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton,
    ImGuiDockNodeFlags_SavedFlagsMask_ = ImGuiDockNodeFlags_NoResizeFlagsMask_ | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_CentralNode | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton,
}ImGuiDockNodeFlagsPrivate_;
typedef enum {
    ImGuiDataAuthority_Auto,
    ImGuiDataAuthority_DockNode,
    ImGuiDataAuthority_Window,
}ImGuiDataAuthority_;
typedef enum {
    ImGuiDockNodeState_Unknown,
    ImGuiDockNodeState_HostWindowHiddenBecauseSingleWindow,
    ImGuiDockNodeState_HostWindowHiddenBecauseWindowsAreResizing,
    ImGuiDockNodeState_HostWindowVisible,
}ImGuiDockNodeState;
typedef struct ImVector_ImGuiWindowPtr {int Size;int Capacity;ImGuiWindow** Data;} ImVector_ImGuiWindowPtr;
struct ImGuiDockNode
{
    ImGuiID ID;
    ImGuiDockNodeFlags SharedFlags;
    ImGuiDockNodeFlags LocalFlags;
    ImGuiDockNodeFlags LocalFlagsInWindows;
    ImGuiDockNodeFlags MergedFlags;
    ImGuiDockNodeState State;
    ImGuiDockNode* ParentNode;
    ImGuiDockNode* ChildNodes[2];
    ImVector_ImGuiWindowPtr Windows;
    ImGuiTabBar* TabBar;
    ImVec2 Pos;
    ImVec2 Size;
    ImVec2 SizeRef;
    ImGuiAxis SplitAxis;
    ImGuiWindowClass WindowClass;
    ImU32 LastBgColor;
    ImGuiWindow* HostWindow;
    ImGuiWindow* VisibleWindow;
    ImGuiDockNode* CentralNode;
    ImGuiDockNode* OnlyNodeWithWindows;
    int CountNodeWithWindows;
    int LastFrameAlive;
    int LastFrameActive;
    int LastFrameFocused;
    ImGuiID LastFocusedNodeId;
    ImGuiID SelectedTabId;
    ImGuiID WantCloseTabId;
    ImGuiID RefViewportId;
    ImGuiDataAuthority AuthorityForPos :3;
    ImGuiDataAuthority AuthorityForSize :3;
    ImGuiDataAuthority AuthorityForViewport :3;
   _Bool         IsVisible :1;
   _Bool         IsFocused :1;
   _Bool         IsBgDrawnThisFrame :1;
   _Bool         HasCloseButton :1;
   _Bool         HasWindowMenuButton :1;
   _Bool         HasCentralNodeChild :1;
   _Bool         WantCloseAll :1;
   _Bool         WantLockSizeOnce :1;
   _Bool         WantMouseMove :1;
   _Bool         WantHiddenTabBarUpdate :1;
   _Bool         WantHiddenTabBarToggle :1;
};
typedef enum {
    ImGuiWindowDockStyleCol_Text,
    ImGuiWindowDockStyleCol_TabHovered,
    ImGuiWindowDockStyleCol_TabFocused,
    ImGuiWindowDockStyleCol_TabSelected,
    ImGuiWindowDockStyleCol_TabSelectedOverline,
    ImGuiWindowDockStyleCol_TabDimmed,
    ImGuiWindowDockStyleCol_TabDimmedSelected,
    ImGuiWindowDockStyleCol_TabDimmedSelectedOverline,
    ImGuiWindowDockStyleCol_COUNT
}ImGuiWindowDockStyleCol;
struct ImGuiWindowDockStyle
{
    ImU32 Colors[ImGuiWindowDockStyleCol_COUNT];
};
typedef struct ImVector_ImGuiDockRequest {int Size;int Capacity;ImGuiDockRequest* Data;} ImVector_ImGuiDockRequest;
typedef struct ImVector_ImGuiDockNodeSettings {int Size;int Capacity;ImGuiDockNodeSettings* Data;} ImVector_ImGuiDockNodeSettings;
struct ImGuiDockContext
{
    ImGuiStorage Nodes;
    ImVector_ImGuiDockRequest Requests;
    ImVector_ImGuiDockNodeSettings NodesSettings;
   _Bool         WantFullRebuild;
};
typedef struct ImGuiViewportP ImGuiViewportP;
struct ImGuiViewportP
{
    ImGuiViewport _ImGuiViewport;
    ImGuiWindow* Window;
    int Idx;
    int LastFrameActive;
    int LastFocusedStampCount;
    ImGuiID LastNameHash;
    ImVec2 LastPos;
    ImVec2 LastSize;
    float Alpha;
    float LastAlpha;
   _Bool         LastFocusedHadNavWindow;
    short PlatformMonitor;
    int BgFgDrawListsLastFrame[2];
    ImDrawList* BgFgDrawLists[2];
    ImDrawData DrawDataP;
    ImDrawDataBuilder DrawDataBuilder;
    ImVec2 LastPlatformPos;
    ImVec2 LastPlatformSize;
    ImVec2 LastRendererSize;
    ImVec2 WorkInsetMin;
    ImVec2 WorkInsetMax;
    ImVec2 BuildWorkInsetMin;
    ImVec2 BuildWorkInsetMax;
};
struct ImGuiWindowSettings
{
    ImGuiID ID;
    ImVec2ih Pos;
    ImVec2ih Size;
    ImVec2ih ViewportPos;
    ImGuiID ViewportId;
    ImGuiID DockId;
    ImGuiID ClassId;
    short DockOrder;
   _Bool         Collapsed;
   _Bool         IsChild;
   _Bool         WantApply;
   _Bool         WantDelete;
};
struct ImGuiSettingsHandler
{
    const char* TypeName;
    ImGuiID TypeHash;
    void (*ClearAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void (*ReadInitFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void* (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);
    void (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line);
    void (*ApplyAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    void (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);
    void* UserData;
};
typedef enum {
ImGuiLocKey_VersionStr=0,
ImGuiLocKey_TableSizeOne=1,
ImGuiLocKey_TableSizeAllFit=2,
ImGuiLocKey_TableSizeAllDefault=3,
ImGuiLocKey_TableResetOrder=4,
ImGuiLocKey_WindowingMainMenuBar=5,
ImGuiLocKey_WindowingPopup=6,
ImGuiLocKey_WindowingUntitled=7,
ImGuiLocKey_OpenLink_s=8,
ImGuiLocKey_CopyLink=9,
ImGuiLocKey_DockingHideTabBar=10,
ImGuiLocKey_DockingHoldShiftToDock=11,
ImGuiLocKey_DockingDragToUndockOrMoveNode=12,
ImGuiLocKey_COUNT=13,
}ImGuiLocKey;
struct ImGuiLocEntry
{
    ImGuiLocKey Key;
    const char* Text;
};
typedef void (*ImGuiErrorCallback)(ImGuiContext* ctx, void* user_data, const char* msg);
typedef enum {
    ImGuiDebugLogFlags_None = 0,
    ImGuiDebugLogFlags_EventError = 1 << 0,
    ImGuiDebugLogFlags_EventActiveId = 1 << 1,
    ImGuiDebugLogFlags_EventFocus = 1 << 2,
    ImGuiDebugLogFlags_EventPopup = 1 << 3,
    ImGuiDebugLogFlags_EventNav = 1 << 4,
    ImGuiDebugLogFlags_EventClipper = 1 << 5,
    ImGuiDebugLogFlags_EventSelection = 1 << 6,
    ImGuiDebugLogFlags_EventIO = 1 << 7,
    ImGuiDebugLogFlags_EventInputRouting = 1 << 8,
    ImGuiDebugLogFlags_EventDocking = 1 << 9,
    ImGuiDebugLogFlags_EventViewport = 1 << 10,
    ImGuiDebugLogFlags_EventMask_ = ImGuiDebugLogFlags_EventError | ImGuiDebugLogFlags_EventActiveId | ImGuiDebugLogFlags_EventFocus | ImGuiDebugLogFlags_EventPopup | ImGuiDebugLogFlags_EventNav | ImGuiDebugLogFlags_EventClipper | ImGuiDebugLogFlags_EventSelection | ImGuiDebugLogFlags_EventIO | ImGuiDebugLogFlags_EventInputRouting | ImGuiDebugLogFlags_EventDocking | ImGuiDebugLogFlags_EventViewport,
    ImGuiDebugLogFlags_OutputToTTY = 1 << 20,
    ImGuiDebugLogFlags_OutputToTestEngine = 1 << 21,
}ImGuiDebugLogFlags_;
typedef struct ImGuiDebugAllocEntry ImGuiDebugAllocEntry;
struct ImGuiDebugAllocEntry
{
    int FrameCount;
    ImS16 AllocCount;
    ImS16 FreeCount;
};
typedef struct ImGuiDebugAllocInfo ImGuiDebugAllocInfo;
struct ImGuiDebugAllocInfo
{
    int TotalAllocCount;
    int TotalFreeCount;
    ImS16 LastEntriesIdx;
    ImGuiDebugAllocEntry LastEntriesBuf[6];
};
struct ImGuiMetricsConfig
{
   _Bool         ShowDebugLog;
   _Bool         ShowIDStackTool;
   _Bool         ShowWindowsRects;
   _Bool         ShowWindowsBeginOrder;
   _Bool         ShowTablesRects;
   _Bool         ShowDrawCmdMesh;
   _Bool         ShowDrawCmdBoundingBoxes;
   _Bool         ShowTextEncodingViewer;
   _Bool         ShowAtlasTintedWithTextColor;
   _Bool         ShowDockingNodes;
    int ShowWindowsRectsType;
    int ShowTablesRectsType;
    int HighlightMonitorIdx;
    ImGuiID HighlightViewportID;
};
typedef struct ImGuiStackLevelInfo ImGuiStackLevelInfo;
struct ImGuiStackLevelInfo
{
    ImGuiID ID;
    ImS8 QueryFrameCount;
   _Bool         QuerySuccess;
    ImGuiDataType DataType : 8;
    char Desc[57];
};
typedef struct ImGuiIDStackTool ImGuiIDStackTool;
typedef struct ImVector_ImGuiStackLevelInfo {int Size;int Capacity;ImGuiStackLevelInfo* Data;} ImVector_ImGuiStackLevelInfo;
struct ImGuiIDStackTool
{
    int LastActiveFrame;
    int StackLevel;
    ImGuiID QueryId;
    ImVector_ImGuiStackLevelInfo Results;
   _Bool         CopyToClipboardOnCtrlC;
    float CopyToClipboardLastTime;
};
typedef void (*ImGuiContextHookCallback)(ImGuiContext* ctx, ImGuiContextHook* hook);
typedef enum { ImGuiContextHookType_NewFramePre, ImGuiContextHookType_NewFramePost, ImGuiContextHookType_EndFramePre, ImGuiContextHookType_EndFramePost, ImGuiContextHookType_RenderPre, ImGuiContextHookType_RenderPost, ImGuiContextHookType_Shutdown, ImGuiContextHookType_PendingRemoval_ }ImGuiContextHookType;
struct ImGuiContextHook
{
    ImGuiID HookId;
    ImGuiContextHookType Type;
    ImGuiID Owner;
    ImGuiContextHookCallback Callback;
    void* UserData;
};
typedef struct ImVector_ImGuiInputEvent {int Size;int Capacity;ImGuiInputEvent* Data;} ImVector_ImGuiInputEvent;
typedef struct ImVector_ImGuiWindowStackData {int Size;int Capacity;ImGuiWindowStackData* Data;} ImVector_ImGuiWindowStackData;
typedef struct ImVector_ImGuiColorMod {int Size;int Capacity;ImGuiColorMod* Data;} ImVector_ImGuiColorMod;
typedef struct ImVector_ImGuiStyleMod {int Size;int Capacity;ImGuiStyleMod* Data;} ImVector_ImGuiStyleMod;
typedef struct ImVector_ImGuiFocusScopeData {int Size;int Capacity;ImGuiFocusScopeData* Data;} ImVector_ImGuiFocusScopeData;
typedef struct ImVector_ImGuiItemFlags {int Size;int Capacity;ImGuiItemFlags* Data;} ImVector_ImGuiItemFlags;
typedef struct ImVector_ImGuiGroupData {int Size;int Capacity;ImGuiGroupData* Data;} ImVector_ImGuiGroupData;
typedef struct ImVector_ImGuiPopupData {int Size;int Capacity;ImGuiPopupData* Data;} ImVector_ImGuiPopupData;
typedef struct ImVector_ImGuiTreeNodeStackData {int Size;int Capacity;ImGuiTreeNodeStackData* Data;} ImVector_ImGuiTreeNodeStackData;
typedef struct ImVector_ImGuiViewportPPtr {int Size;int Capacity;ImGuiViewportP** Data;} ImVector_ImGuiViewportPPtr;
typedef struct ImVector_unsigned_char {int Size;int Capacity;unsigned char* Data;} ImVector_unsigned_char;
typedef struct ImVector_ImGuiListClipperData {int Size;int Capacity;ImGuiListClipperData* Data;} ImVector_ImGuiListClipperData;
typedef struct ImVector_ImGuiTableTempData {int Size;int Capacity;ImGuiTableTempData* Data;} ImVector_ImGuiTableTempData;
typedef struct ImVector_ImGuiTable {int Size;int Capacity;ImGuiTable* Data;} ImVector_ImGuiTable;
typedef struct ImPool_ImGuiTable {ImVector_ImGuiTable Buf;ImGuiStorage Map;ImPoolIdx FreeIdx;ImPoolIdx AliveCount;} ImPool_ImGuiTable;
typedef struct ImVector_ImGuiTabBar {int Size;int Capacity;ImGuiTabBar* Data;} ImVector_ImGuiTabBar;
typedef struct ImPool_ImGuiTabBar {ImVector_ImGuiTabBar Buf;ImGuiStorage Map;ImPoolIdx FreeIdx;ImPoolIdx AliveCount;} ImPool_ImGuiTabBar;
typedef struct ImVector_ImGuiPtrOrIndex {int Size;int Capacity;ImGuiPtrOrIndex* Data;} ImVector_ImGuiPtrOrIndex;
typedef struct ImVector_ImGuiShrinkWidthItem {int Size;int Capacity;ImGuiShrinkWidthItem* Data;} ImVector_ImGuiShrinkWidthItem;
typedef struct ImVector_ImGuiMultiSelectTempData {int Size;int Capacity;ImGuiMultiSelectTempData* Data;} ImVector_ImGuiMultiSelectTempData;
typedef struct ImVector_ImGuiMultiSelectState {int Size;int Capacity;ImGuiMultiSelectState* Data;} ImVector_ImGuiMultiSelectState;
typedef struct ImPool_ImGuiMultiSelectState {ImVector_ImGuiMultiSelectState Buf;ImGuiStorage Map;ImPoolIdx FreeIdx;ImPoolIdx AliveCount;} ImPool_ImGuiMultiSelectState;
typedef struct ImVector_ImGuiID {int Size;int Capacity;ImGuiID* Data;} ImVector_ImGuiID;
typedef struct ImVector_ImGuiSettingsHandler {int Size;int Capacity;ImGuiSettingsHandler* Data;} ImVector_ImGuiSettingsHandler;
typedef struct ImChunkStream_ImGuiWindowSettings {ImVector_char Buf;} ImChunkStream_ImGuiWindowSettings;
typedef struct ImChunkStream_ImGuiTableSettings {ImVector_char Buf;} ImChunkStream_ImGuiTableSettings;
typedef struct ImVector_ImGuiContextHook {int Size;int Capacity;ImGuiContextHook* Data;} ImVector_ImGuiContextHook;
struct ImGuiContext
{
   _Bool         Initialized;
   _Bool         FontAtlasOwnedByContext;
    ImGuiIO IO;
    ImGuiPlatformIO PlatformIO;
    ImGuiStyle Style;
    ImGuiConfigFlags ConfigFlagsCurrFrame;
    ImGuiConfigFlags ConfigFlagsLastFrame;
    ImFont* Font;
    float FontSize;
    float FontBaseSize;
    float FontScale;
    float CurrentDpiScale;
    ImDrawListSharedData DrawListSharedData;
    double Time;
    int FrameCount;
    int FrameCountEnded;
    int FrameCountPlatformEnded;
    int FrameCountRendered;
   _Bool         WithinFrameScope;
   _Bool         WithinFrameScopeWithImplicitWindow;
   _Bool         WithinEndChild;
   _Bool         GcCompactAll;
   _Bool         TestEngineHookItems;
    void* TestEngine;
    char ContextName[16];
    ImVector_ImGuiInputEvent InputEventsQueue;
    ImVector_ImGuiInputEvent InputEventsTrail;
    ImGuiMouseSource InputEventsNextMouseSource;
    ImU32 InputEventsNextEventId;
    ImVector_ImGuiWindowPtr Windows;
    ImVector_ImGuiWindowPtr WindowsFocusOrder;
    ImVector_ImGuiWindowPtr WindowsTempSortBuffer;
    ImVector_ImGuiWindowStackData CurrentWindowStack;
    ImGuiStorage WindowsById;
    int WindowsActiveCount;
    ImVec2 WindowsHoverPadding;
    ImGuiID DebugBreakInWindow;
    ImGuiWindow* CurrentWindow;
    ImGuiWindow* HoveredWindow;
    ImGuiWindow* HoveredWindowUnderMovingWindow;
    ImGuiWindow* HoveredWindowBeforeClear;
    ImGuiWindow* MovingWindow;
    ImGuiWindow* WheelingWindow;
    ImVec2 WheelingWindowRefMousePos;
    int WheelingWindowStartFrame;
    int WheelingWindowScrolledFrame;
    float WheelingWindowReleaseTimer;
    ImVec2 WheelingWindowWheelRemainder;
    ImVec2 WheelingAxisAvg;
    ImGuiID DebugDrawIdConflicts;
    ImGuiID DebugHookIdInfo;
    ImGuiID HoveredId;
    ImGuiID HoveredIdPreviousFrame;
    int HoveredIdPreviousFrameItemCount;
    float HoveredIdTimer;
    float HoveredIdNotActiveTimer;
   _Bool         HoveredIdAllowOverlap;
   _Bool         HoveredIdIsDisabled;
   _Bool         ItemUnclipByLog;
    ImGuiID ActiveId;
    ImGuiID ActiveIdIsAlive;
    float ActiveIdTimer;
   _Bool         ActiveIdIsJustActivated;
   _Bool         ActiveIdAllowOverlap;
   _Bool         ActiveIdNoClearOnFocusLoss;
   _Bool         ActiveIdHasBeenPressedBefore;
   _Bool         ActiveIdHasBeenEditedBefore;
   _Bool         ActiveIdHasBeenEditedThisFrame;
   _Bool         ActiveIdFromShortcut;
    int ActiveIdMouseButton : 8;
    ImVec2 ActiveIdClickOffset;
    ImGuiWindow* ActiveIdWindow;
    ImGuiInputSource ActiveIdSource;
    ImGuiID ActiveIdPreviousFrame;
   _Bool         ActiveIdPreviousFrameIsAlive;
   _Bool         ActiveIdPreviousFrameHasBeenEditedBefore;
    ImGuiWindow* ActiveIdPreviousFrameWindow;
    ImGuiID LastActiveId;
    float LastActiveIdTimer;
    double LastKeyModsChangeTime;
    double LastKeyModsChangeFromNoneTime;
    double LastKeyboardKeyPressTime;
    ImBitArrayForNamedKeys KeysMayBeCharInput;
    ImGuiKeyOwnerData KeysOwnerData[ImGuiKey_NamedKey_COUNT];
    ImGuiKeyRoutingTable KeysRoutingTable;
    ImU32 ActiveIdUsingNavDirMask;
   _Bool         ActiveIdUsingAllKeyboardKeys;
    ImGuiKeyChord DebugBreakInShortcutRouting;
    ImGuiID CurrentFocusScopeId;
    ImGuiItemFlags CurrentItemFlags;
    ImGuiID DebugLocateId;
    ImGuiNextItemData NextItemData;
    ImGuiLastItemData LastItemData;
    ImGuiNextWindowData NextWindowData;
   _Bool         DebugShowGroupRects;
    ImGuiCol DebugFlashStyleColorIdx;
    ImVector_ImGuiColorMod ColorStack;
    ImVector_ImGuiStyleMod StyleVarStack;
    ImVector_ImFontPtr FontStack;
    ImVector_ImGuiFocusScopeData FocusScopeStack;
    ImVector_ImGuiItemFlags ItemFlagsStack;
    ImVector_ImGuiGroupData GroupStack;
    ImVector_ImGuiPopupData OpenPopupStack;
    ImVector_ImGuiPopupData BeginPopupStack;
    ImVector_ImGuiTreeNodeStackData TreeNodeStack;
    ImVector_ImGuiViewportPPtr Viewports;
    ImGuiViewportP* CurrentViewport;
    ImGuiViewportP* MouseViewport;
    ImGuiViewportP* MouseLastHoveredViewport;
    ImGuiID PlatformLastFocusedViewportId;
    ImGuiPlatformMonitor FallbackMonitor;
    ImRect PlatformMonitorsFullWorkRect;
    int ViewportCreatedCount;
    int PlatformWindowsCreatedCount;
    int ViewportFocusedStampCount;
   _Bool         NavCursorVisible;
   _Bool         NavHighlightItemUnderNav;
   _Bool         NavMousePosDirty;
   _Bool         NavIdIsAlive;
    ImGuiID NavId;
    ImGuiWindow* NavWindow;
    ImGuiID NavFocusScopeId;
    ImGuiNavLayer NavLayer;
    ImGuiID NavActivateId;
    ImGuiID NavActivateDownId;
    ImGuiID NavActivatePressedId;
    ImGuiActivateFlags NavActivateFlags;
    ImVector_ImGuiFocusScopeData NavFocusRoute;
    ImGuiID NavHighlightActivatedId;
    float NavHighlightActivatedTimer;
    ImGuiID NavNextActivateId;
    ImGuiActivateFlags NavNextActivateFlags;
    ImGuiInputSource NavInputSource;
    ImGuiSelectionUserData NavLastValidSelectionUserData;
    ImS8 NavCursorHideFrames;
   _Bool         NavAnyRequest;
   _Bool         NavInitRequest;
   _Bool         NavInitRequestFromMove;
    ImGuiNavItemData NavInitResult;
   _Bool         NavMoveSubmitted;
   _Bool         NavMoveScoringItems;
   _Bool         NavMoveForwardToNextFrame;
    ImGuiNavMoveFlags NavMoveFlags;
    ImGuiScrollFlags NavMoveScrollFlags;
    ImGuiKeyChord NavMoveKeyMods;
    ImGuiDir NavMoveDir;
    ImGuiDir NavMoveDirForDebug;
    ImGuiDir NavMoveClipDir;
    ImRect NavScoringRect;
    ImRect NavScoringNoClipRect;
    int NavScoringDebugCount;
    int NavTabbingDir;
    int NavTabbingCounter;
    ImGuiNavItemData NavMoveResultLocal;
    ImGuiNavItemData NavMoveResultLocalVisible;
    ImGuiNavItemData NavMoveResultOther;
    ImGuiNavItemData NavTabbingResultFirst;
    ImGuiID NavJustMovedFromFocusScopeId;
    ImGuiID NavJustMovedToId;
    ImGuiID NavJustMovedToFocusScopeId;
    ImGuiKeyChord NavJustMovedToKeyMods;
   _Bool         NavJustMovedToIsTabbing;
   _Bool         NavJustMovedToHasSelectionData;
    ImGuiKeyChord ConfigNavWindowingKeyNext;
    ImGuiKeyChord ConfigNavWindowingKeyPrev;
    ImGuiWindow* NavWindowingTarget;
    ImGuiWindow* NavWindowingTargetAnim;
    ImGuiWindow* NavWindowingListWindow;
    float NavWindowingTimer;
    float NavWindowingHighlightAlpha;
   _Bool         NavWindowingToggleLayer;
    ImGuiKey NavWindowingToggleKey;
    ImVec2 NavWindowingAccumDeltaPos;
    ImVec2 NavWindowingAccumDeltaSize;
    float DimBgRatio;
   _Bool         DragDropActive;
   _Bool         DragDropWithinSource;
   _Bool         DragDropWithinTarget;
    ImGuiDragDropFlags DragDropSourceFlags;
    int DragDropSourceFrameCount;
    int DragDropMouseButton;
    ImGuiPayload DragDropPayload;
    ImRect DragDropTargetRect;
    ImRect DragDropTargetClipRect;
    ImGuiID DragDropTargetId;
    ImGuiDragDropFlags DragDropAcceptFlags;
    float DragDropAcceptIdCurrRectSurface;
    ImGuiID DragDropAcceptIdCurr;
    ImGuiID DragDropAcceptIdPrev;
    int DragDropAcceptFrameCount;
    ImGuiID DragDropHoldJustPressedId;
    ImVector_unsigned_char DragDropPayloadBufHeap;
    unsigned char DragDropPayloadBufLocal[16];
    int ClipperTempDataStacked;
    ImVector_ImGuiListClipperData ClipperTempData;
    ImGuiTable* CurrentTable;
    ImGuiID DebugBreakInTable;
    int TablesTempDataStacked;
    ImVector_ImGuiTableTempData TablesTempData;
    ImPool_ImGuiTable Tables;
    ImVector_float TablesLastTimeActive;
    ImVector_ImDrawChannel DrawChannelsTempMergeBuffer;
    ImGuiTabBar* CurrentTabBar;
    ImPool_ImGuiTabBar TabBars;
    ImVector_ImGuiPtrOrIndex CurrentTabBarStack;
    ImVector_ImGuiShrinkWidthItem ShrinkWidthBuffer;
    ImGuiBoxSelectState BoxSelectState;
    ImGuiMultiSelectTempData* CurrentMultiSelect;
    int MultiSelectTempDataStacked;
    ImVector_ImGuiMultiSelectTempData MultiSelectTempData;
    ImPool_ImGuiMultiSelectState MultiSelectStorage;
    ImGuiID HoverItemDelayId;
    ImGuiID HoverItemDelayIdPreviousFrame;
    float HoverItemDelayTimer;
    float HoverItemDelayClearTimer;
    ImGuiID HoverItemUnlockedStationaryId;
    ImGuiID HoverWindowUnlockedStationaryId;
    ImGuiMouseCursor MouseCursor;
    float MouseStationaryTimer;
    ImVec2 MouseLastValidPos;
    ImGuiInputTextState InputTextState;
    ImGuiInputTextDeactivatedState InputTextDeactivatedState;
    ImFont InputTextPasswordFont;
    ImGuiID TempInputId;
    ImGuiDataTypeStorage DataTypeZeroValue;
    int BeginMenuDepth;
    int BeginComboDepth;
    ImGuiColorEditFlags ColorEditOptions;
    ImGuiID ColorEditCurrentID;
    ImGuiID ColorEditSavedID;
    float ColorEditSavedHue;
    float ColorEditSavedSat;
    ImU32 ColorEditSavedColor;
    ImVec4 ColorPickerRef;
    ImGuiComboPreviewData ComboPreviewData;
    ImRect WindowResizeBorderExpectedRect;
   _Bool         WindowResizeRelativeMode;
    short ScrollbarSeekMode;
    float ScrollbarClickDeltaToGrabCenter;
    float SliderGrabClickOffset;
    float SliderCurrentAccum;
   _Bool         SliderCurrentAccumDirty;
   _Bool         DragCurrentAccumDirty;
    float DragCurrentAccum;
    float DragSpeedDefaultRatio;
    float DisabledAlphaBackup;
    short DisabledStackSize;
    short TooltipOverrideCount;
    ImGuiWindow* TooltipPreviousWindow;
    ImVector_char ClipboardHandlerData;
    ImVector_ImGuiID MenusIdSubmittedThisFrame;
    ImGuiTypingSelectState TypingSelectState;
    ImGuiPlatformImeData PlatformImeData;
    ImGuiPlatformImeData PlatformImeDataPrev;
    ImGuiID PlatformImeViewport;
    ImGuiDockContext DockContext;
    void (*DockNodeWindowMenuHandler)(ImGuiContext* ctx, ImGuiDockNode* node, ImGuiTabBar* tab_bar);
   _Bool         SettingsLoaded;
    float SettingsDirtyTimer;
    ImGuiTextBuffer SettingsIniData;
    ImVector_ImGuiSettingsHandler SettingsHandlers;
    ImChunkStream_ImGuiWindowSettings SettingsWindows;
    ImChunkStream_ImGuiTableSettings SettingsTables;
    ImVector_ImGuiContextHook Hooks;
    ImGuiID HookIdNext;
    const char* LocalizationTable[ImGuiLocKey_COUNT];
   _Bool         LogEnabled;
    ImGuiLogType LogType;
    ImFileHandle LogFile;
    ImGuiTextBuffer LogBuffer;
    const char* LogNextPrefix;
    const char* LogNextSuffix;
    float LogLinePosY;
   _Bool         LogLineFirstItem;
    int LogDepthRef;
    int LogDepthToExpand;
    int LogDepthToExpandDefault;
    ImGuiErrorCallback ErrorCallback;
    void* ErrorCallbackUserData;
    ImVec2 ErrorTooltipLockedPos;
   _Bool         ErrorFirst;
    int ErrorCountCurrentFrame;
    ImGuiErrorRecoveryState StackSizesInNewFrame;
    ImGuiErrorRecoveryState*StackSizesInBeginForCurrentWindow;
    int DebugDrawIdConflictsCount;
    ImGuiDebugLogFlags DebugLogFlags;
    ImGuiTextBuffer DebugLogBuf;
    ImGuiTextIndex DebugLogIndex;
    int DebugLogSkippedErrors;
    ImGuiDebugLogFlags DebugLogAutoDisableFlags;
    ImU8 DebugLogAutoDisableFrames;
    ImU8 DebugLocateFrames;
   _Bool         DebugBreakInLocateId;
    ImGuiKeyChord DebugBreakKeyChord;
    ImS8 DebugBeginReturnValueCullDepth;
   _Bool         DebugItemPickerActive;
    ImU8 DebugItemPickerMouseButton;
    ImGuiID DebugItemPickerBreakId;
    float DebugFlashStyleColorTime;
    ImVec4 DebugFlashStyleColorBackup;
    ImGuiMetricsConfig DebugMetricsConfig;
    ImGuiIDStackTool DebugIDStackTool;
    ImGuiDebugAllocInfo DebugAllocInfo;
    ImGuiDockNode* DebugHoveredDockNode;
    float FramerateSecPerFrame[60];
    int FramerateSecPerFrameIdx;
    int FramerateSecPerFrameCount;
    float FramerateSecPerFrameAccum;
    int WantCaptureMouseNextFrame;
    int WantCaptureKeyboardNextFrame;
    int WantTextInputNextFrame;
    ImVector_char TempBuffer;
    char TempKeychordName[64];
};
struct ImGuiWindowTempData
{
    ImVec2 CursorPos;
    ImVec2 CursorPosPrevLine;
    ImVec2 CursorStartPos;
    ImVec2 CursorMaxPos;
    ImVec2 IdealMaxPos;
    ImVec2 CurrLineSize;
    ImVec2 PrevLineSize;
    float CurrLineTextBaseOffset;
    float PrevLineTextBaseOffset;
   _Bool         IsSameLine;
   _Bool         IsSetPos;
    ImVec1 Indent;
    ImVec1 ColumnsOffset;
    ImVec1 GroupOffset;
    ImVec2 CursorStartPosLossyness;
    ImGuiNavLayer NavLayerCurrent;
    short NavLayersActiveMask;
    short NavLayersActiveMaskNext;
   _Bool         NavIsScrollPushableX;
   _Bool         NavHideHighlightOneFrame;
   _Bool         NavWindowHasScrollY;
   _Bool         MenuBarAppending;
    ImVec2 MenuBarOffset;
    ImGuiMenuColumns MenuColumns;
    int TreeDepth;
    ImU32 TreeHasStackDataDepthMask;
    ImVector_ImGuiWindowPtr ChildWindows;
    ImGuiStorage* StateStorage;
    ImGuiOldColumns* CurrentColumns;
    int CurrentTableIdx;
    ImGuiLayoutType LayoutType;
    ImGuiLayoutType ParentLayoutType;
    ImU32 ModalDimBgColor;
    float ItemWidth;
    float TextWrapPos;
    ImVector_float ItemWidthStack;
    ImVector_float TextWrapPosStack;
};
typedef struct ImVector_ImGuiOldColumns {int Size;int Capacity;ImGuiOldColumns* Data;} ImVector_ImGuiOldColumns;
struct ImGuiWindow
{
    ImGuiContext* Ctx;
    char* Name;
    ImGuiID ID;
    ImGuiWindowFlags Flags, FlagsPreviousFrame;
    ImGuiChildFlags ChildFlags;
    ImGuiWindowClass WindowClass;
    ImGuiViewportP* Viewport;
    ImGuiID ViewportId;
    ImVec2 ViewportPos;
    int ViewportAllowPlatformMonitorExtend;
    ImVec2 Pos;
    ImVec2 Size;
    ImVec2 SizeFull;
    ImVec2 ContentSize;
    ImVec2 ContentSizeIdeal;
    ImVec2 ContentSizeExplicit;
    ImVec2 WindowPadding;
    float WindowRounding;
    float WindowBorderSize;
    float TitleBarHeight, MenuBarHeight;
    float DecoOuterSizeX1, DecoOuterSizeY1;
    float DecoOuterSizeX2, DecoOuterSizeY2;
    float DecoInnerSizeX1, DecoInnerSizeY1;
    int NameBufLen;
    ImGuiID MoveId;
    ImGuiID TabId;
    ImGuiID ChildId;
    ImGuiID PopupId;
    ImVec2 Scroll;
    ImVec2 ScrollMax;
    ImVec2 ScrollTarget;
    ImVec2 ScrollTargetCenterRatio;
    ImVec2 ScrollTargetEdgeSnapDist;
    ImVec2 ScrollbarSizes;
   _Bool         ScrollbarX, ScrollbarY;
   _Bool         ViewportOwned;
   _Bool         Active;
   _Bool         WasActive;
   _Bool         WriteAccessed;
   _Bool         Collapsed;
   _Bool         WantCollapseToggle;
   _Bool         SkipItems;
   _Bool         SkipRefresh;
   _Bool         Appearing;
   _Bool         Hidden;
   _Bool         IsFallbackWindow;
   _Bool         IsExplicitChild;
   _Bool         HasCloseButton;
    signed char ResizeBorderHovered;
    signed char ResizeBorderHeld;
    short BeginCount;
    short BeginCountPreviousFrame;
    short BeginOrderWithinParent;
    short BeginOrderWithinContext;
    short FocusOrder;
    ImS8 AutoFitFramesX, AutoFitFramesY;
   _Bool         AutoFitOnlyGrows;
    ImGuiDir AutoPosLastDirection;
    ImS8 HiddenFramesCanSkipItems;
    ImS8 HiddenFramesCannotSkipItems;
    ImS8 HiddenFramesForRenderOnly;
    ImS8 DisableInputsFrames;
    ImGuiCond SetWindowPosAllowFlags : 8;
    ImGuiCond SetWindowSizeAllowFlags : 8;
    ImGuiCond SetWindowCollapsedAllowFlags : 8;
    ImGuiCond SetWindowDockAllowFlags : 8;
    ImVec2 SetWindowPosVal;
    ImVec2 SetWindowPosPivot;
    ImVector_ImGuiID IDStack;
    ImGuiWindowTempData DC;
    ImRect OuterRectClipped;
    ImRect InnerRect;
    ImRect InnerClipRect;
    ImRect WorkRect;
    ImRect ParentWorkRect;
    ImRect ClipRect;
    ImRect ContentRegionRect;
    ImVec2ih HitTestHoleSize;
    ImVec2ih HitTestHoleOffset;
    int LastFrameActive;
    int LastFrameJustFocused;
    float LastTimeActive;
    float ItemWidthDefault;
    ImGuiStorage StateStorage;
    ImVector_ImGuiOldColumns ColumnsStorage;
    float FontWindowScale;
    float FontDpiScale;
    int SettingsOffset;
    ImDrawList* DrawList;
    ImDrawList DrawListInst;
    ImGuiWindow* ParentWindow;
    ImGuiWindow* ParentWindowInBeginStack;
    ImGuiWindow* RootWindow;
    ImGuiWindow* RootWindowPopupTree;
    ImGuiWindow* RootWindowDockTree;
    ImGuiWindow* RootWindowForTitleBarHighlight;
    ImGuiWindow* RootWindowForNav;
    ImGuiWindow* ParentWindowForFocusRoute;
    ImGuiWindow* NavLastChildNavWindow;
    ImGuiID NavLastIds[ImGuiNavLayer_COUNT];
    ImRect NavRectRel[ImGuiNavLayer_COUNT];
    ImVec2 NavPreferredScoringPosRel[ImGuiNavLayer_COUNT];
    ImGuiID NavRootFocusScopeId;
    int MemoryDrawListIdxCapacity;
    int MemoryDrawListVtxCapacity;
   _Bool         MemoryCompacted;
   _Bool         DockIsActive :1;
   _Bool         DockNodeIsVisible :1;
   _Bool         DockTabIsVisible :1;
   _Bool         DockTabWantClose :1;
    short DockOrder;
    ImGuiWindowDockStyle DockStyle;
    ImGuiDockNode* DockNode;
    ImGuiDockNode* DockNodeAsHost;
    ImGuiID DockId;
    ImGuiItemStatusFlags DockTabItemStatusFlags;
    ImRect DockTabItemRect;
};
typedef enum {
    ImGuiTabBarFlags_DockNode = 1 << 20,
    ImGuiTabBarFlags_IsFocused = 1 << 21,
    ImGuiTabBarFlags_SaveSettings = 1 << 22,
}ImGuiTabBarFlagsPrivate_;
typedef enum {
    ImGuiTabItemFlags_SectionMask_ = ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_Trailing,
    ImGuiTabItemFlags_NoCloseButton = 1 << 20,
    ImGuiTabItemFlags_Button = 1 << 21,
    ImGuiTabItemFlags_Unsorted = 1 << 22,
}ImGuiTabItemFlagsPrivate_;
struct ImGuiTabItem
{
    ImGuiID ID;
    ImGuiTabItemFlags Flags;
    ImGuiWindow* Window;
    int LastFrameVisible;
    int LastFrameSelected;
    float Offset;
    float Width;
    float ContentWidth;
    float RequestedWidth;
    ImS32 NameOffset;
    ImS16 BeginOrder;
    ImS16 IndexDuringLayout;
   _Bool         WantClose;
};
typedef struct ImVector_ImGuiTabItem {int Size;int Capacity;ImGuiTabItem* Data;} ImVector_ImGuiTabItem;
struct ImGuiTabBar
{
    ImGuiWindow* Window;
    ImVector_ImGuiTabItem Tabs;
    ImGuiTabBarFlags Flags;
    ImGuiID ID;
    ImGuiID SelectedTabId;
    ImGuiID NextSelectedTabId;
    ImGuiID VisibleTabId;
    int CurrFrameVisible;
    int PrevFrameVisible;
    ImRect BarRect;
    float CurrTabsContentsHeight;
    float PrevTabsContentsHeight;
    float WidthAllTabs;
    float WidthAllTabsIdeal;
    float ScrollingAnim;
    float ScrollingTarget;
    float ScrollingTargetDistToVisibility;
    float ScrollingSpeed;
    float ScrollingRectMinX;
    float ScrollingRectMaxX;
    float SeparatorMinX;
    float SeparatorMaxX;
    ImGuiID ReorderRequestTabId;
    ImS16 ReorderRequestOffset;
    ImS8 BeginCount;
   _Bool         WantLayout;
   _Bool         VisibleTabWasSubmitted;
   _Bool         TabsAddedNew;
    ImS16 TabsActiveCount;
    ImS16 LastTabItemIdx;
    float ItemSpacingY;
    ImVec2 FramePadding;
    ImVec2 BackupCursorPos;
    ImGuiTextBuffer TabsNames;
};
typedef ImS16 ImGuiTableColumnIdx;
typedef ImU16 ImGuiTableDrawChannelIdx;
struct ImGuiTableColumn
{
    ImGuiTableColumnFlags Flags;
    float WidthGiven;
    float MinX;
    float MaxX;
    float WidthRequest;
    float WidthAuto;
    float WidthMax;
    float StretchWeight;
    float InitStretchWeightOrWidth;
    ImRect ClipRect;
    ImGuiID UserID;
    float WorkMinX;
    float WorkMaxX;
    float ItemWidth;
    float ContentMaxXFrozen;
    float ContentMaxXUnfrozen;
    float ContentMaxXHeadersUsed;
    float ContentMaxXHeadersIdeal;
    ImS16 NameOffset;
    ImGuiTableColumnIdx DisplayOrder;
    ImGuiTableColumnIdx IndexWithinEnabledSet;
    ImGuiTableColumnIdx PrevEnabledColumn;
    ImGuiTableColumnIdx NextEnabledColumn;
    ImGuiTableColumnIdx SortOrder;
    ImGuiTableDrawChannelIdx DrawChannelCurrent;
    ImGuiTableDrawChannelIdx DrawChannelFrozen;
    ImGuiTableDrawChannelIdx DrawChannelUnfrozen;
   _Bool         IsEnabled;
   _Bool         IsUserEnabled;
   _Bool         IsUserEnabledNextFrame;
   _Bool         IsVisibleX;
   _Bool         IsVisibleY;
   _Bool         IsRequestOutput;
   _Bool         IsSkipItems;
   _Bool         IsPreserveWidthAuto;
    ImS8 NavLayerCurrent;
    ImU8 AutoFitQueue;
    ImU8 CannotSkipItemsQueue;
    ImU8 SortDirection : 2;
    ImU8 SortDirectionsAvailCount : 2;
    ImU8 SortDirectionsAvailMask : 4;
    ImU8 SortDirectionsAvailList;
};
typedef struct ImGuiTableCellData ImGuiTableCellData;
struct ImGuiTableCellData
{
    ImU32 BgColor;
    ImGuiTableColumnIdx Column;
};
struct ImGuiTableHeaderData
{
    ImGuiTableColumnIdx Index;
    ImU32 TextColor;
    ImU32 BgColor0;
    ImU32 BgColor1;
};
struct ImGuiTableInstanceData
{
    ImGuiID TableInstanceID;
    float LastOuterHeight;
    float LastTopHeadersRowHeight;
    float LastFrozenHeight;
    int HoveredRowLast;
    int HoveredRowNext;
};
typedef struct ImSpan_ImGuiTableColumn {ImGuiTableColumn* Data;ImGuiTableColumn* DataEnd;} ImSpan_ImGuiTableColumn;
typedef struct ImSpan_ImGuiTableColumnIdx {ImGuiTableColumnIdx* Data;ImGuiTableColumnIdx* DataEnd;} ImSpan_ImGuiTableColumnIdx;
typedef struct ImSpan_ImGuiTableCellData {ImGuiTableCellData* Data;ImGuiTableCellData* DataEnd;} ImSpan_ImGuiTableCellData;
typedef struct ImVector_ImGuiTableInstanceData {int Size;int Capacity;ImGuiTableInstanceData* Data;} ImVector_ImGuiTableInstanceData;
typedef struct ImVector_ImGuiTableColumnSortSpecs {int Size;int Capacity;ImGuiTableColumnSortSpecs* Data;} ImVector_ImGuiTableColumnSortSpecs;
struct ImGuiTable
{
    ImGuiID ID;
    ImGuiTableFlags Flags;
    void* RawData;
    ImGuiTableTempData* TempData;
    ImSpan_ImGuiTableColumn Columns;
    ImSpan_ImGuiTableColumnIdx DisplayOrderToIndex;
    ImSpan_ImGuiTableCellData RowCellData;
    ImBitArrayPtr EnabledMaskByDisplayOrder;
    ImBitArrayPtr EnabledMaskByIndex;
    ImBitArrayPtr VisibleMaskByIndex;
    ImGuiTableFlags SettingsLoadedFlags;
    int SettingsOffset;
    int LastFrameActive;
    int ColumnsCount;
    int CurrentRow;
    int CurrentColumn;
    ImS16 InstanceCurrent;
    ImS16 InstanceInteracted;
    float RowPosY1;
    float RowPosY2;
    float RowMinHeight;
    float RowCellPaddingY;
    float RowTextBaseline;
    float RowIndentOffsetX;
    ImGuiTableRowFlags RowFlags : 16;
    ImGuiTableRowFlags LastRowFlags : 16;
    int RowBgColorCounter;
    ImU32 RowBgColor[2];
    ImU32 BorderColorStrong;
    ImU32 BorderColorLight;
    float BorderX1;
    float BorderX2;
    float HostIndentX;
    float MinColumnWidth;
    float OuterPaddingX;
    float CellPaddingX;
    float CellSpacingX1;
    float CellSpacingX2;
    float InnerWidth;
    float ColumnsGivenWidth;
    float ColumnsAutoFitWidth;
    float ColumnsStretchSumWeights;
    float ResizedColumnNextWidth;
    float ResizeLockMinContentsX2;
    float RefScale;
    float AngledHeadersHeight;
    float AngledHeadersSlope;
    ImRect OuterRect;
    ImRect InnerRect;
    ImRect WorkRect;
    ImRect InnerClipRect;
    ImRect BgClipRect;
    ImRect Bg0ClipRectForDrawCmd;
    ImRect Bg2ClipRectForDrawCmd;
    ImRect HostClipRect;
    ImRect HostBackupInnerClipRect;
    ImGuiWindow* OuterWindow;
    ImGuiWindow* InnerWindow;
    ImGuiTextBuffer ColumnsNames;
    ImDrawListSplitter* DrawSplitter;
    ImGuiTableInstanceData InstanceDataFirst;
    ImVector_ImGuiTableInstanceData InstanceDataExtra;
    ImGuiTableColumnSortSpecs SortSpecsSingle;
    ImVector_ImGuiTableColumnSortSpecs SortSpecsMulti;
    ImGuiTableSortSpecs SortSpecs;
    ImGuiTableColumnIdx SortSpecsCount;
    ImGuiTableColumnIdx ColumnsEnabledCount;
    ImGuiTableColumnIdx ColumnsEnabledFixedCount;
    ImGuiTableColumnIdx DeclColumnsCount;
    ImGuiTableColumnIdx AngledHeadersCount;
    ImGuiTableColumnIdx HoveredColumnBody;
    ImGuiTableColumnIdx HoveredColumnBorder;
    ImGuiTableColumnIdx HighlightColumnHeader;
    ImGuiTableColumnIdx AutoFitSingleColumn;
    ImGuiTableColumnIdx ResizedColumn;
    ImGuiTableColumnIdx LastResizedColumn;
    ImGuiTableColumnIdx HeldHeaderColumn;
    ImGuiTableColumnIdx ReorderColumn;
    ImGuiTableColumnIdx ReorderColumnDir;
    ImGuiTableColumnIdx LeftMostEnabledColumn;
    ImGuiTableColumnIdx RightMostEnabledColumn;
    ImGuiTableColumnIdx LeftMostStretchedColumn;
    ImGuiTableColumnIdx RightMostStretchedColumn;
    ImGuiTableColumnIdx ContextPopupColumn;
    ImGuiTableColumnIdx FreezeRowsRequest;
    ImGuiTableColumnIdx FreezeRowsCount;
    ImGuiTableColumnIdx FreezeColumnsRequest;
    ImGuiTableColumnIdx FreezeColumnsCount;
    ImGuiTableColumnIdx RowCellDataCurrent;
    ImGuiTableDrawChannelIdx DummyDrawChannel;
    ImGuiTableDrawChannelIdx Bg2DrawChannelCurrent;
    ImGuiTableDrawChannelIdx Bg2DrawChannelUnfrozen;
   _Bool         IsLayoutLocked;
   _Bool         IsInsideRow;
   _Bool         IsInitializing;
   _Bool         IsSortSpecsDirty;
   _Bool         IsUsingHeaders;
   _Bool         IsContextPopupOpen;
   _Bool         DisableDefaultContextMenu;
   _Bool         IsSettingsRequestLoad;
   _Bool         IsSettingsDirty;
   _Bool         IsDefaultDisplayOrder;
   _Bool         IsResetAllRequest;
   _Bool         IsResetDisplayOrderRequest;
   _Bool         IsUnfrozenRows;
   _Bool         IsDefaultSizingPolicy;
   _Bool         IsActiveIdAliveBeforeTable;
   _Bool         IsActiveIdInTable;
   _Bool         HasScrollbarYCurr;
   _Bool         HasScrollbarYPrev;
   _Bool         MemoryCompacted;
   _Bool         HostSkipItems;
};
typedef struct ImVector_ImGuiTableHeaderData {int Size;int Capacity;ImGuiTableHeaderData* Data;} ImVector_ImGuiTableHeaderData;
struct ImGuiTableTempData
{
    int TableIndex;
    float LastTimeActive;
    float AngledHeadersExtraWidth;
    ImVector_ImGuiTableHeaderData AngledHeadersRequests;
    ImVec2 UserOuterSize;
    ImDrawListSplitter DrawSplitter;
    ImRect HostBackupWorkRect;
    ImRect HostBackupParentWorkRect;
    ImVec2 HostBackupPrevLineSize;
    ImVec2 HostBackupCurrLineSize;
    ImVec2 HostBackupCursorMaxPos;
    ImVec1 HostBackupColumnsOffset;
    float HostBackupItemWidth;
    int HostBackupItemWidthStackSize;
};
typedef struct ImGuiTableColumnSettings ImGuiTableColumnSettings;
struct ImGuiTableColumnSettings
{
    float WidthOrWeight;
    ImGuiID UserID;
    ImGuiTableColumnIdx Index;
    ImGuiTableColumnIdx DisplayOrder;
    ImGuiTableColumnIdx SortOrder;
    ImU8 SortDirection : 2;
    ImU8 IsEnabled : 1;
    ImU8 IsStretch : 1;
};
struct ImGuiTableSettings
{
    ImGuiID ID;
    ImGuiTableFlags SaveFlags;
    float RefScale;
    ImGuiTableColumnIdx ColumnsCount;
    ImGuiTableColumnIdx ColumnsCountMax;
   _Bool         WantApply;
};
struct ImFontBuilderIO
{
   _Bool         (*FontBuilder_Build)(ImFontAtlas* atlas);
};
ImVec2* ImVec2_ImVec2_Nil(void);
void ImVec2_destroy(ImVec2* self);
ImVec2* ImVec2_ImVec2_Float(float _x,float _y);
ImVec4* ImVec4_ImVec4_Nil(void);
void ImVec4_destroy(ImVec4* self);
ImVec4* ImVec4_ImVec4_Float(float _x,float _y,float _z,float _w);
ImGuiContext* igCreateContext(ImFontAtlas* shared_font_atlas);
void igDestroyContext(ImGuiContext* ctx);
ImGuiContext* igGetCurrentContext(void);
void igSetCurrentContext(ImGuiContext* ctx);
ImGuiIO* igGetIO(void);
ImGuiPlatformIO* igGetPlatformIO(void);
ImGuiStyle* igGetStyle(void);
void igNewFrame(void);
void igEndFrame(void);
void igRender(void);
ImDrawData* igGetDrawData(void);
void igShowDemoWindow(                                _Bool                                    * p_open);
void igShowMetricsWindow(                                   _Bool                                       * p_open);
void igShowDebugLogWindow(                                    _Bool                                        * p_open);
void igShowIDStackToolWindow(                                       _Bool                                           * p_open);
void igShowAboutWindow(                                 _Bool                                     * p_open);
void igShowStyleEditor(ImGuiStyle* ref);
_Bool                igShowStyleSelector(const char* label);
void igShowFontSelector(const char* label);
void igShowUserGuide(void);
const char* igGetVersion(void);
void igStyleColorsDark(ImGuiStyle* dst);
void igStyleColorsLight(ImGuiStyle* dst);
void igStyleColorsClassic(ImGuiStyle* dst);
_Bool                igBegin(const char* name,                                        _Bool                                            * p_open,ImGuiWindowFlags flags);
void igEnd(void);
_Bool                igBeginChild_Str(const char* str_id,const ImVec2 size,ImGuiChildFlags child_flags,ImGuiWindowFlags window_flags);
_Bool                igBeginChild_ID(ImGuiID id,const ImVec2 size,ImGuiChildFlags child_flags,ImGuiWindowFlags window_flags);
void igEndChild(void);
_Bool                igIsWindowAppearing(void);
_Bool                igIsWindowCollapsed(void);
_Bool                igIsWindowFocused(ImGuiFocusedFlags flags);
_Bool                igIsWindowHovered(ImGuiHoveredFlags flags);
ImDrawList* igGetWindowDrawList(void);
float igGetWindowDpiScale(void);
void igGetWindowPos(ImVec2 *pOut);
void igGetWindowSize(ImVec2 *pOut);
float igGetWindowWidth(void);
float igGetWindowHeight(void);
ImGuiViewport* igGetWindowViewport(void);
void igSetNextWindowPos(const ImVec2 pos,ImGuiCond cond,const ImVec2 pivot);
void igSetNextWindowSize(const ImVec2 size,ImGuiCond cond);
void igSetNextWindowSizeConstraints(const ImVec2 size_min,const ImVec2 size_max,ImGuiSizeCallback custom_callback,void* custom_callback_data);
void igSetNextWindowContentSize(const ImVec2 size);
void igSetNextWindowCollapsed(                                        _Bool                                              collapsed,ImGuiCond cond);
void igSetNextWindowFocus(void);
void igSetNextWindowScroll(const ImVec2 scroll);
void igSetNextWindowBgAlpha(float alpha);
void igSetNextWindowViewport(ImGuiID viewport_id);
void igSetWindowPos_Vec2(const ImVec2 pos,ImGuiCond cond);
void igSetWindowSize_Vec2(const ImVec2 size,ImGuiCond cond);
void igSetWindowCollapsed_Bool(                                         _Bool                                               collapsed,ImGuiCond cond);
void igSetWindowFocus_Nil(void);
void igSetWindowFontScale(float scale);
void igSetWindowPos_Str(const char* name,const ImVec2 pos,ImGuiCond cond);
void igSetWindowSize_Str(const char* name,const ImVec2 size,ImGuiCond cond);
void igSetWindowCollapsed_Str(const char* name,                                                         _Bool                                                               collapsed,ImGuiCond cond);
void igSetWindowFocus_Str(const char* name);
float igGetScrollX(void);
float igGetScrollY(void);
void igSetScrollX_Float(float scroll_x);
void igSetScrollY_Float(float scroll_y);
float igGetScrollMaxX(void);
float igGetScrollMaxY(void);
void igSetScrollHereX(float center_x_ratio);
void igSetScrollHereY(float center_y_ratio);
void igSetScrollFromPosX_Float(float local_x,float center_x_ratio);
void igSetScrollFromPosY_Float(float local_y,float center_y_ratio);
void igPushFont(ImFont* font);
void igPopFont(void);
void igPushStyleColor_U32(ImGuiCol idx,ImU32 col);
void igPushStyleColor_Vec4(ImGuiCol idx,const ImVec4 col);
void igPopStyleColor(int count);
void igPushStyleVar_Float(ImGuiStyleVar idx,float val);
void igPushStyleVar_Vec2(ImGuiStyleVar idx,const ImVec2 val);
void igPushStyleVarX(ImGuiStyleVar idx,float val_x);
void igPushStyleVarY(ImGuiStyleVar idx,float val_y);
void igPopStyleVar(int count);
void igPushItemFlag(ImGuiItemFlags option,                                                    _Bool                                                          enabled);
void igPopItemFlag(void);
void igPushItemWidth(float item_width);
void igPopItemWidth(void);
void igSetNextItemWidth(float item_width);
float igCalcItemWidth(void);
void igPushTextWrapPos(float wrap_local_pos_x);
void igPopTextWrapPos(void);
ImFont* igGetFont(void);
float igGetFontSize(void);
void igGetFontTexUvWhitePixel(ImVec2 *pOut);
ImU32 igGetColorU32_Col(ImGuiCol idx,float alpha_mul);
ImU32 igGetColorU32_Vec4(const ImVec4 col);
ImU32 igGetColorU32_U32(ImU32 col,float alpha_mul);
const ImVec4* igGetStyleColorVec4(ImGuiCol idx);
void igGetCursorScreenPos(ImVec2 *pOut);
void igSetCursorScreenPos(const ImVec2 pos);
void igGetContentRegionAvail(ImVec2 *pOut);
void igGetCursorPos(ImVec2 *pOut);
float igGetCursorPosX(void);
float igGetCursorPosY(void);
void igSetCursorPos(const ImVec2 local_pos);
void igSetCursorPosX(float local_x);
void igSetCursorPosY(float local_y);
void igGetCursorStartPos(ImVec2 *pOut);
void igSeparator(void);
void igSameLine(float offset_from_start_x,float spacing);
void igNewLine(void);
void igSpacing(void);
void igDummy(const ImVec2 size);
void igIndent(float indent_w);
void igUnindent(float indent_w);
void igBeginGroup(void);
void igEndGroup(void);
void igAlignTextToFramePadding(void);
float igGetTextLineHeight(void);
float igGetTextLineHeightWithSpacing(void);
float igGetFrameHeight(void);
float igGetFrameHeightWithSpacing(void);
void igPushID_Str(const char* str_id);
void igPushID_StrStr(const char* str_id_begin,const char* str_id_end);
void igPushID_Ptr(const void* ptr_id);
void igPushID_Int(int int_id);
void igPopID(void);
ImGuiID igGetID_Str(const char* str_id);
ImGuiID igGetID_StrStr(const char* str_id_begin,const char* str_id_end);
ImGuiID igGetID_Ptr(const void* ptr_id);
ImGuiID igGetID_Int(int int_id);
void igTextUnformatted(const char* text,const char* text_end);
void igText(const char* fmt,...);
void igTextV(const char* fmt,va_list args);
void igTextColored(const ImVec4 col,const char* fmt,...);
void igTextColoredV(const ImVec4 col,const char* fmt,va_list args);
void igTextDisabled(const char* fmt,...);
void igTextDisabledV(const char* fmt,va_list args);
void igTextWrapped(const char* fmt,...);
void igTextWrappedV(const char* fmt,va_list args);
void igLabelText(const char* label,const char* fmt,...);
void igLabelTextV(const char* label,const char* fmt,va_list args);
void igBulletText(const char* fmt,...);
void igBulletTextV(const char* fmt,va_list args);
void igSeparatorText(const char* label);
_Bool                igButton(const char* label,const ImVec2 size);
_Bool                igSmallButton(const char* label);
_Bool                igInvisibleButton(const char* str_id,const ImVec2 size,ImGuiButtonFlags flags);
_Bool                igArrowButton(const char* str_id,ImGuiDir dir);
_Bool                igCheckbox(const char* label,                                            _Bool                                                * v);
_Bool                igCheckboxFlags_IntPtr(const char* label,int* flags,int flags_value);
_Bool                igCheckboxFlags_UintPtr(const char* label,unsigned int* flags,unsigned int flags_value);
_Bool                igRadioButton_Bool(const char* label,                                                    _Bool                                                          active);
_Bool                igRadioButton_IntPtr(const char* label,int* v,int v_button);
void igProgressBar(float fraction,const ImVec2 size_arg,const char* overlay);
void igBullet(void);
_Bool                igTextLink(const char* label);
void igTextLinkOpenURL(const char* label,const char* url);
void igImage(ImTextureID user_texture_id,const ImVec2 image_size,const ImVec2 uv0,const ImVec2 uv1,const ImVec4 tint_col,const ImVec4 border_col);
_Bool                igImageButton(const char* str_id,ImTextureID user_texture_id,const ImVec2 image_size,const ImVec2 uv0,const ImVec2 uv1,const ImVec4 bg_col,const ImVec4 tint_col);
_Bool                igBeginCombo(const char* label,const char* preview_value,ImGuiComboFlags flags);
void igEndCombo(void);
_Bool                igCombo_Str_arr(const char* label,int* current_item,const char* const items[],int items_count,int popup_max_height_in_items);
_Bool                igCombo_Str(const char* label,int* current_item,const char* items_separated_by_zeros,int popup_max_height_in_items);
_Bool                igCombo_FnStrPtr(const char* label,int* current_item,const char*(*getter)(void* user_data,int idx),void* user_data,int items_count,int popup_max_height_in_items);
_Bool                igDragFloat(const char* label,float* v,float v_speed,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragFloat2(const char* label,float v[2],float v_speed,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragFloat3(const char* label,float v[3],float v_speed,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragFloat4(const char* label,float v[4],float v_speed,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragFloatRange2(const char* label,float* v_current_min,float* v_current_max,float v_speed,float v_min,float v_max,const char* format,const char* format_max,ImGuiSliderFlags flags);
_Bool                igDragInt(const char* label,int* v,float v_speed,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragInt2(const char* label,int v[2],float v_speed,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragInt3(const char* label,int v[3],float v_speed,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragInt4(const char* label,int v[4],float v_speed,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragIntRange2(const char* label,int* v_current_min,int* v_current_max,float v_speed,int v_min,int v_max,const char* format,const char* format_max,ImGuiSliderFlags flags);
_Bool                igDragScalar(const char* label,ImGuiDataType data_type,void* p_data,float v_speed,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igDragScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,float v_speed,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderFloat(const char* label,float* v,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderFloat2(const char* label,float v[2],float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderFloat3(const char* label,float v[3],float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderFloat4(const char* label,float v[4],float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderAngle(const char* label,float* v_rad,float v_degrees_min,float v_degrees_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderInt(const char* label,int* v,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderInt2(const char* label,int v[2],int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderInt3(const char* label,int v[3],int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderInt4(const char* label,int v[4],int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderScalar(const char* label,ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igVSliderFloat(const char* label,const ImVec2 size,float* v,float v_min,float v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igVSliderInt(const char* label,const ImVec2 size,int* v,int v_min,int v_max,const char* format,ImGuiSliderFlags flags);
_Bool                igVSliderScalar(const char* label,const ImVec2 size,ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igInputText(const char* label,char* buf,size_t buf_size,ImGuiInputTextFlags flags,ImGuiInputTextCallback callback,void* user_data);
_Bool                igInputTextMultiline(const char* label,char* buf,size_t buf_size,const ImVec2 size,ImGuiInputTextFlags flags,ImGuiInputTextCallback callback,void* user_data);
_Bool                igInputTextWithHint(const char* label,const char* hint,char* buf,size_t buf_size,ImGuiInputTextFlags flags,ImGuiInputTextCallback callback,void* user_data);
_Bool                igInputFloat(const char* label,float* v,float step,float step_fast,const char* format,ImGuiInputTextFlags flags);
_Bool                igInputFloat2(const char* label,float v[2],const char* format,ImGuiInputTextFlags flags);
_Bool                igInputFloat3(const char* label,float v[3],const char* format,ImGuiInputTextFlags flags);
_Bool                igInputFloat4(const char* label,float v[4],const char* format,ImGuiInputTextFlags flags);
_Bool                igInputInt(const char* label,int* v,int step,int step_fast,ImGuiInputTextFlags flags);
_Bool                igInputInt2(const char* label,int v[2],ImGuiInputTextFlags flags);
_Bool                igInputInt3(const char* label,int v[3],ImGuiInputTextFlags flags);
_Bool                igInputInt4(const char* label,int v[4],ImGuiInputTextFlags flags);
_Bool                igInputDouble(const char* label,double* v,double step,double step_fast,const char* format,ImGuiInputTextFlags flags);
_Bool                igInputScalar(const char* label,ImGuiDataType data_type,void* p_data,const void* p_step,const void* p_step_fast,const char* format,ImGuiInputTextFlags flags);
_Bool                igInputScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,const void* p_step,const void* p_step_fast,const char* format,ImGuiInputTextFlags flags);
_Bool                igColorEdit3(const char* label,float col[3],ImGuiColorEditFlags flags);
_Bool                igColorEdit4(const char* label,float col[4],ImGuiColorEditFlags flags);
_Bool                igColorPicker3(const char* label,float col[3],ImGuiColorEditFlags flags);
_Bool                igColorPicker4(const char* label,float col[4],ImGuiColorEditFlags flags,const float* ref_col);
_Bool                igColorButton(const char* desc_id,const ImVec4 col,ImGuiColorEditFlags flags,const ImVec2 size);
void igSetColorEditOptions(ImGuiColorEditFlags flags);
_Bool                igTreeNode_Str(const char* label);
_Bool                igTreeNode_StrStr(const char* str_id,const char* fmt,...);
_Bool                igTreeNode_Ptr(const void* ptr_id,const char* fmt,...);
_Bool                igTreeNodeV_Str(const char* str_id,const char* fmt,va_list args);
_Bool                igTreeNodeV_Ptr(const void* ptr_id,const char* fmt,va_list args);
_Bool                igTreeNodeEx_Str(const char* label,ImGuiTreeNodeFlags flags);
_Bool                igTreeNodeEx_StrStr(const char* str_id,ImGuiTreeNodeFlags flags,const char* fmt,...);
_Bool                igTreeNodeEx_Ptr(const void* ptr_id,ImGuiTreeNodeFlags flags,const char* fmt,...);
_Bool                igTreeNodeExV_Str(const char* str_id,ImGuiTreeNodeFlags flags,const char* fmt,va_list args);
_Bool                igTreeNodeExV_Ptr(const void* ptr_id,ImGuiTreeNodeFlags flags,const char* fmt,va_list args);
void igTreePush_Str(const char* str_id);
void igTreePush_Ptr(const void* ptr_id);
void igTreePop(void);
float igGetTreeNodeToLabelSpacing(void);
_Bool                igCollapsingHeader_TreeNodeFlags(const char* label,ImGuiTreeNodeFlags flags);
_Bool                igCollapsingHeader_BoolPtr(const char* label,                                                            _Bool                                                                * p_visible,ImGuiTreeNodeFlags flags);
void igSetNextItemOpen(                                 _Bool                                       is_open,ImGuiCond cond);
void igSetNextItemStorageID(ImGuiID storage_id);
_Bool                igSelectable_Bool(const char* label,                                                   _Bool                                                         selected,ImGuiSelectableFlags flags,const ImVec2 size);
_Bool                igSelectable_BoolPtr(const char* label,                                                      _Bool                                                          * p_selected,ImGuiSelectableFlags flags,const ImVec2 size);
ImGuiMultiSelectIO* igBeginMultiSelect(ImGuiMultiSelectFlags flags,int selection_size,int items_count);
ImGuiMultiSelectIO* igEndMultiSelect(void);
void igSetNextItemSelectionUserData(ImGuiSelectionUserData selection_user_data);
_Bool                igIsItemToggledSelection(void);
_Bool                igBeginListBox(const char* label,const ImVec2 size);
void igEndListBox(void);
_Bool                igListBox_Str_arr(const char* label,int* current_item,const char* const items[],int items_count,int height_in_items);
_Bool                igListBox_FnStrPtr(const char* label,int* current_item,const char*(*getter)(void* user_data,int idx),void* user_data,int items_count,int height_in_items);
void igPlotLines_FloatPtr(const char* label,const float* values,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,ImVec2 graph_size,int stride);
void igPlotLines_FnFloatPtr(const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,ImVec2 graph_size);
void igPlotHistogram_FloatPtr(const char* label,const float* values,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,ImVec2 graph_size,int stride);
void igPlotHistogram_FnFloatPtr(const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,ImVec2 graph_size);
void igValue_Bool(const char* prefix,                                               _Bool                                                     b);
void igValue_Int(const char* prefix,int v);
void igValue_Uint(const char* prefix,unsigned int v);
void igValue_Float(const char* prefix,float v,const char* float_format);
_Bool                igBeginMenuBar(void);
void igEndMenuBar(void);
_Bool                igBeginMainMenuBar(void);
void igEndMainMenuBar(void);
_Bool                igBeginMenu(const char* label,                                             _Bool                                                   enabled);
void igEndMenu(void);
_Bool                igMenuItem_Bool(const char* label,const char* shortcut,                                                                      _Bool                                                                            selected,                                                                                    _Bool                                                                                          enabled);
_Bool                igMenuItem_BoolPtr(const char* label,const char* shortcut,                                                                         _Bool                                                                             * p_selected,                                                                                          _Bool                                                                                                enabled);
_Bool                igBeginTooltip(void);
void igEndTooltip(void);
void igSetTooltip(const char* fmt,...);
void igSetTooltipV(const char* fmt,va_list args);
_Bool                igBeginItemTooltip(void);
void igSetItemTooltip(const char* fmt,...);
void igSetItemTooltipV(const char* fmt,va_list args);
_Bool                igBeginPopup(const char* str_id,ImGuiWindowFlags flags);
_Bool                igBeginPopupModal(const char* name,                                                  _Bool                                                      * p_open,ImGuiWindowFlags flags);
void igEndPopup(void);
void igOpenPopup_Str(const char* str_id,ImGuiPopupFlags popup_flags);
void igOpenPopup_ID(ImGuiID id,ImGuiPopupFlags popup_flags);
void igOpenPopupOnItemClick(const char* str_id,ImGuiPopupFlags popup_flags);
void igCloseCurrentPopup(void);
_Bool                igBeginPopupContextItem(const char* str_id,ImGuiPopupFlags popup_flags);
_Bool                igBeginPopupContextWindow(const char* str_id,ImGuiPopupFlags popup_flags);
_Bool                igBeginPopupContextVoid(const char* str_id,ImGuiPopupFlags popup_flags);
_Bool                igIsPopupOpen_Str(const char* str_id,ImGuiPopupFlags flags);
_Bool                igBeginTable(const char* str_id,int columns,ImGuiTableFlags flags,const ImVec2 outer_size,float inner_width);
void igEndTable(void);
void igTableNextRow(ImGuiTableRowFlags row_flags,float min_row_height);
_Bool                igTableNextColumn(void);
_Bool                igTableSetColumnIndex(int column_n);
void igTableSetupColumn(const char* label,ImGuiTableColumnFlags flags,float init_width_or_weight,ImGuiID user_id);
void igTableSetupScrollFreeze(int cols,int rows);
void igTableHeader(const char* label);
void igTableHeadersRow(void);
void igTableAngledHeadersRow(void);
ImGuiTableSortSpecs* igTableGetSortSpecs(void);
int igTableGetColumnCount(void);
int igTableGetColumnIndex(void);
int igTableGetRowIndex(void);
const char* igTableGetColumnName_Int(int column_n);
ImGuiTableColumnFlags igTableGetColumnFlags(int column_n);
void igTableSetColumnEnabled(int column_n,                                                    _Bool                                                          v);
int igTableGetHoveredColumn(void);
void igTableSetBgColor(ImGuiTableBgTarget target,ImU32 color,int column_n);
void igColumns(int count,const char* id,                                                  _Bool                                                        borders);
void igNextColumn(void);
int igGetColumnIndex(void);
float igGetColumnWidth(int column_index);
void igSetColumnWidth(int column_index,float width);
float igGetColumnOffset(int column_index);
void igSetColumnOffset(int column_index,float offset_x);
int igGetColumnsCount(void);
_Bool                igBeginTabBar(const char* str_id,ImGuiTabBarFlags flags);
void igEndTabBar(void);
_Bool                igBeginTabItem(const char* label,                                                _Bool                                                    * p_open,ImGuiTabItemFlags flags);
void igEndTabItem(void);
_Bool                igTabItemButton(const char* label,ImGuiTabItemFlags flags);
void igSetTabItemClosed(const char* tab_or_docked_window_label);
ImGuiID igDockSpace(ImGuiID dockspace_id,const ImVec2 size,ImGuiDockNodeFlags flags,const ImGuiWindowClass* window_class);
ImGuiID igDockSpaceOverViewport(ImGuiID dockspace_id,const ImGuiViewport* viewport,ImGuiDockNodeFlags flags,const ImGuiWindowClass* window_class);
void igSetNextWindowDockID(ImGuiID dock_id,ImGuiCond cond);
void igSetNextWindowClass(const ImGuiWindowClass* window_class);
ImGuiID igGetWindowDockID(void);
_Bool                igIsWindowDocked(void);
void igLogToTTY(int auto_open_depth);
void igLogToFile(int auto_open_depth,const char* filename);
void igLogToClipboard(int auto_open_depth);
void igLogFinish(void);
void igLogButtons(void);
void igLogTextV(const char* fmt,va_list args);
_Bool                igBeginDragDropSource(ImGuiDragDropFlags flags);
_Bool                igSetDragDropPayload(const char* type,const void* data,size_t sz,ImGuiCond cond);
void igEndDragDropSource(void);
_Bool                igBeginDragDropTarget(void);
const ImGuiPayload* igAcceptDragDropPayload(const char* type,ImGuiDragDropFlags flags);
void igEndDragDropTarget(void);
const ImGuiPayload* igGetDragDropPayload(void);
void igBeginDisabled(                               _Bool                                     disabled);
void igEndDisabled(void);
void igPushClipRect(const ImVec2 clip_rect_min,const ImVec2 clip_rect_max,                                                                                    _Bool                                                                                          intersect_with_current_clip_rect);
void igPopClipRect(void);
void igSetItemDefaultFocus(void);
void igSetKeyboardFocusHere(int offset);
void igSetNavCursorVisible(                                     _Bool                                           visible);
void igSetNextItemAllowOverlap(void);
_Bool                igIsItemHovered(ImGuiHoveredFlags flags);
_Bool                igIsItemActive(void);
_Bool                igIsItemFocused(void);
_Bool                igIsItemClicked(ImGuiMouseButton mouse_button);
_Bool                igIsItemVisible(void);
_Bool                igIsItemEdited(void);
_Bool                igIsItemActivated(void);
_Bool                igIsItemDeactivated(void);
_Bool                igIsItemDeactivatedAfterEdit(void);
_Bool                igIsItemToggledOpen(void);
_Bool                igIsAnyItemHovered(void);
_Bool                igIsAnyItemActive(void);
_Bool                igIsAnyItemFocused(void);
ImGuiID igGetItemID(void);
void igGetItemRectMin(ImVec2 *pOut);
void igGetItemRectMax(ImVec2 *pOut);
void igGetItemRectSize(ImVec2 *pOut);
ImGuiViewport* igGetMainViewport(void);
ImDrawList* igGetBackgroundDrawList(ImGuiViewport* viewport);
ImDrawList* igGetForegroundDrawList_ViewportPtr(ImGuiViewport* viewport);
_Bool                igIsRectVisible_Nil(const ImVec2 size);
_Bool                igIsRectVisible_Vec2(const ImVec2 rect_min,const ImVec2 rect_max);
double igGetTime(void);
int igGetFrameCount(void);
ImDrawListSharedData* igGetDrawListSharedData(void);
const char* igGetStyleColorName(ImGuiCol idx);
void igSetStateStorage(ImGuiStorage* storage);
ImGuiStorage* igGetStateStorage(void);
void igCalcTextSize(ImVec2 *pOut,const char* text,const char* text_end,                                                                                 _Bool                                                                                       hide_text_after_double_hash,float wrap_width);
void igColorConvertU32ToFloat4(ImVec4 *pOut,ImU32 in);
ImU32 igColorConvertFloat4ToU32(const ImVec4 in);
void igColorConvertRGBtoHSV(float r,float g,float b,float* out_h,float* out_s,float* out_v);
void igColorConvertHSVtoRGB(float h,float s,float v,float* out_r,float* out_g,float* out_b);
_Bool                igIsKeyDown_Nil(ImGuiKey key);
_Bool                igIsKeyPressed_Bool(ImGuiKey key,                                                _Bool                                                      repeat);
_Bool                igIsKeyReleased_Nil(ImGuiKey key);
_Bool                igIsKeyChordPressed_Nil(ImGuiKeyChord key_chord);
int igGetKeyPressedAmount(ImGuiKey key,float repeat_delay,float rate);
const char* igGetKeyName(ImGuiKey key);
void igSetNextFrameWantCaptureKeyboard(                                                 _Bool                                                       want_capture_keyboard);
_Bool                igShortcut_Nil(ImGuiKeyChord key_chord,ImGuiInputFlags flags);
void igSetNextItemShortcut(ImGuiKeyChord key_chord,ImGuiInputFlags flags);
void igSetItemKeyOwner_Nil(ImGuiKey key);
_Bool                igIsMouseDown_Nil(ImGuiMouseButton button);
_Bool                igIsMouseClicked_Bool(ImGuiMouseButton button,                                                             _Bool                                                                   repeat);
_Bool                igIsMouseReleased_Nil(ImGuiMouseButton button);
_Bool                igIsMouseDoubleClicked_Nil(ImGuiMouseButton button);
int igGetMouseClickedCount(ImGuiMouseButton button);
_Bool                igIsMouseHoveringRect(const ImVec2 r_min,const ImVec2 r_max,                                                                           _Bool                                                                                 clip);
_Bool                igIsMousePosValid(const ImVec2* mouse_pos);
_Bool                igIsAnyMouseDown(void);
void igGetMousePos(ImVec2 *pOut);
void igGetMousePosOnOpeningCurrentPopup(ImVec2 *pOut);
_Bool                igIsMouseDragging(ImGuiMouseButton button,float lock_threshold);
void igGetMouseDragDelta(ImVec2 *pOut,ImGuiMouseButton button,float lock_threshold);
void igResetMouseDragDelta(ImGuiMouseButton button);
ImGuiMouseCursor igGetMouseCursor(void);
void igSetMouseCursor(ImGuiMouseCursor cursor_type);
void igSetNextFrameWantCaptureMouse(                                              _Bool                                                    want_capture_mouse);
const char* igGetClipboardText(void);
void igSetClipboardText(const char* text);
void igLoadIniSettingsFromDisk(const char* ini_filename);
void igLoadIniSettingsFromMemory(const char* ini_data,size_t ini_size);
void igSaveIniSettingsToDisk(const char* ini_filename);
const char* igSaveIniSettingsToMemory(size_t* out_ini_size);
void igDebugTextEncoding(const char* text);
void igDebugFlashStyleColor(ImGuiCol idx);
void igDebugStartItemPicker(void);
_Bool                igDebugCheckVersionAndDataLayout(const char* version_str,size_t sz_io,size_t sz_style,size_t sz_vec2,size_t sz_vec4,size_t sz_drawvert,size_t sz_drawidx);
void igDebugLog(const char* fmt,...);
void igDebugLogV(const char* fmt,va_list args);
void igSetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,ImGuiMemFreeFunc free_func,void* user_data);
void igGetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,ImGuiMemFreeFunc* p_free_func,void** p_user_data);
void* igMemAlloc(size_t size);
void igMemFree(void* ptr);
void igUpdatePlatformWindows(void);
void igRenderPlatformWindowsDefault(void* platform_render_arg,void* renderer_render_arg);
void igDestroyPlatformWindows(void);
ImGuiViewport* igFindViewportByID(ImGuiID id);
ImGuiViewport* igFindViewportByPlatformHandle(void* platform_handle);
ImGuiTableSortSpecs* ImGuiTableSortSpecs_ImGuiTableSortSpecs(void);
void ImGuiTableSortSpecs_destroy(ImGuiTableSortSpecs* self);
ImGuiTableColumnSortSpecs* ImGuiTableColumnSortSpecs_ImGuiTableColumnSortSpecs(void);
void ImGuiTableColumnSortSpecs_destroy(ImGuiTableColumnSortSpecs* self);
ImGuiStyle* ImGuiStyle_ImGuiStyle(void);
void ImGuiStyle_destroy(ImGuiStyle* self);
void ImGuiStyle_ScaleAllSizes(ImGuiStyle* self,float scale_factor);
void ImGuiIO_AddKeyEvent(ImGuiIO* self,ImGuiKey key,                                                              _Bool                                                                    down);
void ImGuiIO_AddKeyAnalogEvent(ImGuiIO* self,ImGuiKey key,                                                                    _Bool                                                                          down,float v);
void ImGuiIO_AddMousePosEvent(ImGuiIO* self,float x,float y);
void ImGuiIO_AddMouseButtonEvent(ImGuiIO* self,int button,                                                                    _Bool                                                                          down);
void ImGuiIO_AddMouseWheelEvent(ImGuiIO* self,float wheel_x,float wheel_y);
void ImGuiIO_AddMouseSourceEvent(ImGuiIO* self,ImGuiMouseSource source);
void ImGuiIO_AddMouseViewportEvent(ImGuiIO* self,ImGuiID id);
void ImGuiIO_AddFocusEvent(ImGuiIO* self,                                                   _Bool                                                         focused);
void ImGuiIO_AddInputCharacter(ImGuiIO* self,unsigned int c);
void ImGuiIO_AddInputCharacterUTF16(ImGuiIO* self,ImWchar16 c);
void ImGuiIO_AddInputCharactersUTF8(ImGuiIO* self,const char* str);
void ImGuiIO_SetKeyEventNativeData(ImGuiIO* self,ImGuiKey key,int native_keycode,int native_scancode,int native_legacy_index);
void ImGuiIO_SetAppAcceptingEvents(ImGuiIO* self,                                                           _Bool                                                                 accepting_events);
void ImGuiIO_ClearEventsQueue(ImGuiIO* self);
void ImGuiIO_ClearInputKeys(ImGuiIO* self);
void ImGuiIO_ClearInputMouse(ImGuiIO* self);
ImGuiIO* ImGuiIO_ImGuiIO(void);
void ImGuiIO_destroy(ImGuiIO* self);
ImGuiInputTextCallbackData* ImGuiInputTextCallbackData_ImGuiInputTextCallbackData(void);
void ImGuiInputTextCallbackData_destroy(ImGuiInputTextCallbackData* self);
void ImGuiInputTextCallbackData_DeleteChars(ImGuiInputTextCallbackData* self,int pos,int bytes_count);
void ImGuiInputTextCallbackData_InsertChars(ImGuiInputTextCallbackData* self,int pos,const char* text,const char* text_end);
void ImGuiInputTextCallbackData_SelectAll(ImGuiInputTextCallbackData* self);
void ImGuiInputTextCallbackData_ClearSelection(ImGuiInputTextCallbackData* self);
_Bool                ImGuiInputTextCallbackData_HasSelection(ImGuiInputTextCallbackData* self);
ImGuiWindowClass* ImGuiWindowClass_ImGuiWindowClass(void);
void ImGuiWindowClass_destroy(ImGuiWindowClass* self);
ImGuiPayload* ImGuiPayload_ImGuiPayload(void);
void ImGuiPayload_destroy(ImGuiPayload* self);
void ImGuiPayload_Clear(ImGuiPayload* self);
_Bool                ImGuiPayload_IsDataType(ImGuiPayload* self,const char* type);
_Bool                ImGuiPayload_IsPreview(ImGuiPayload* self);
_Bool                ImGuiPayload_IsDelivery(ImGuiPayload* self);
ImGuiOnceUponAFrame* ImGuiOnceUponAFrame_ImGuiOnceUponAFrame(void);
void ImGuiOnceUponAFrame_destroy(ImGuiOnceUponAFrame* self);
ImGuiTextFilter* ImGuiTextFilter_ImGuiTextFilter(const char* default_filter);
void ImGuiTextFilter_destroy(ImGuiTextFilter* self);
_Bool                ImGuiTextFilter_Draw(ImGuiTextFilter* self,const char* label,float width);
_Bool                ImGuiTextFilter_PassFilter(ImGuiTextFilter* self,const char* text,const char* text_end);
void ImGuiTextFilter_Build(ImGuiTextFilter* self);
void ImGuiTextFilter_Clear(ImGuiTextFilter* self);
_Bool                ImGuiTextFilter_IsActive(ImGuiTextFilter* self);
ImGuiTextRange* ImGuiTextRange_ImGuiTextRange_Nil(void);
void ImGuiTextRange_destroy(ImGuiTextRange* self);
ImGuiTextRange* ImGuiTextRange_ImGuiTextRange_Str(const char* _b,const char* _e);
_Bool                ImGuiTextRange_empty(ImGuiTextRange* self);
void ImGuiTextRange_split(ImGuiTextRange* self,char separator,ImVector_ImGuiTextRange* out);
ImGuiTextBuffer* ImGuiTextBuffer_ImGuiTextBuffer(void);
void ImGuiTextBuffer_destroy(ImGuiTextBuffer* self);
const char* ImGuiTextBuffer_begin(ImGuiTextBuffer* self);
const char* ImGuiTextBuffer_end(ImGuiTextBuffer* self);
int ImGuiTextBuffer_size(ImGuiTextBuffer* self);
_Bool                ImGuiTextBuffer_empty(ImGuiTextBuffer* self);
void ImGuiTextBuffer_clear(ImGuiTextBuffer* self);
void ImGuiTextBuffer_reserve(ImGuiTextBuffer* self,int capacity);
const char* ImGuiTextBuffer_c_str(ImGuiTextBuffer* self);
void ImGuiTextBuffer_append(ImGuiTextBuffer* self,const char* str,const char* str_end);
void ImGuiTextBuffer_appendfv(ImGuiTextBuffer* self,const char* fmt,va_list args);
ImGuiStoragePair* ImGuiStoragePair_ImGuiStoragePair_Int(ImGuiID _key,int _val);
void ImGuiStoragePair_destroy(ImGuiStoragePair* self);
ImGuiStoragePair* ImGuiStoragePair_ImGuiStoragePair_Float(ImGuiID _key,float _val);
ImGuiStoragePair* ImGuiStoragePair_ImGuiStoragePair_Ptr(ImGuiID _key,void* _val);
void ImGuiStorage_Clear(ImGuiStorage* self);
int ImGuiStorage_GetInt(ImGuiStorage* self,ImGuiID key,int default_val);
void ImGuiStorage_SetInt(ImGuiStorage* self,ImGuiID key,int val);
_Bool                ImGuiStorage_GetBool(ImGuiStorage* self,ImGuiID key,                                                                   _Bool                                                                         default_val);
void ImGuiStorage_SetBool(ImGuiStorage* self,ImGuiID key,                                                                   _Bool                                                                         val);
float ImGuiStorage_GetFloat(ImGuiStorage* self,ImGuiID key,float default_val);
void ImGuiStorage_SetFloat(ImGuiStorage* self,ImGuiID key,float val);
void* ImGuiStorage_GetVoidPtr(ImGuiStorage* self,ImGuiID key);
void ImGuiStorage_SetVoidPtr(ImGuiStorage* self,ImGuiID key,void* val);
int* ImGuiStorage_GetIntRef(ImGuiStorage* self,ImGuiID key,int default_val);
_Bool              * ImGuiStorage_GetBoolRef(ImGuiStorage* self,ImGuiID key,                                                                       _Bool                                                                             default_val);
float* ImGuiStorage_GetFloatRef(ImGuiStorage* self,ImGuiID key,float default_val);
void** ImGuiStorage_GetVoidPtrRef(ImGuiStorage* self,ImGuiID key,void* default_val);
void ImGuiStorage_BuildSortByKey(ImGuiStorage* self);
void ImGuiStorage_SetAllInt(ImGuiStorage* self,int val);
ImGuiListClipper* ImGuiListClipper_ImGuiListClipper(void);
void ImGuiListClipper_destroy(ImGuiListClipper* self);
void ImGuiListClipper_Begin(ImGuiListClipper* self,int items_count,float items_height);
void ImGuiListClipper_End(ImGuiListClipper* self);
_Bool                ImGuiListClipper_Step(ImGuiListClipper* self);
void ImGuiListClipper_IncludeItemByIndex(ImGuiListClipper* self,int item_index);
void ImGuiListClipper_IncludeItemsByIndex(ImGuiListClipper* self,int item_begin,int item_end);
void ImGuiListClipper_SeekCursorForItem(ImGuiListClipper* self,int item_index);
ImColor* ImColor_ImColor_Nil(void);
void ImColor_destroy(ImColor* self);
ImColor* ImColor_ImColor_Float(float r,float g,float b,float a);
ImColor* ImColor_ImColor_Vec4(const ImVec4 col);
ImColor* ImColor_ImColor_Int(int r,int g,int b,int a);
ImColor* ImColor_ImColor_U32(ImU32 rgba);
void ImColor_SetHSV(ImColor* self,float h,float s,float v,float a);
void ImColor_HSV(ImColor *pOut,float h,float s,float v,float a);
ImGuiSelectionBasicStorage* ImGuiSelectionBasicStorage_ImGuiSelectionBasicStorage(void);
void ImGuiSelectionBasicStorage_destroy(ImGuiSelectionBasicStorage* self);
void ImGuiSelectionBasicStorage_ApplyRequests(ImGuiSelectionBasicStorage* self,ImGuiMultiSelectIO* ms_io);
_Bool                ImGuiSelectionBasicStorage_Contains(ImGuiSelectionBasicStorage* self,ImGuiID id);
void ImGuiSelectionBasicStorage_Clear(ImGuiSelectionBasicStorage* self);
void ImGuiSelectionBasicStorage_Swap(ImGuiSelectionBasicStorage* self,ImGuiSelectionBasicStorage* r);
void ImGuiSelectionBasicStorage_SetItemSelected(ImGuiSelectionBasicStorage* self,ImGuiID id,                                                                                                      _Bool                                                                                                            selected);
_Bool                ImGuiSelectionBasicStorage_GetNextSelectedItem(ImGuiSelectionBasicStorage* self,void** opaque_it,ImGuiID* out_id);
ImGuiID ImGuiSelectionBasicStorage_GetStorageIdFromIndex(ImGuiSelectionBasicStorage* self,int idx);
ImGuiSelectionExternalStorage* ImGuiSelectionExternalStorage_ImGuiSelectionExternalStorage(void);
void ImGuiSelectionExternalStorage_destroy(ImGuiSelectionExternalStorage* self);
void ImGuiSelectionExternalStorage_ApplyRequests(ImGuiSelectionExternalStorage* self,ImGuiMultiSelectIO* ms_io);
ImDrawCmd* ImDrawCmd_ImDrawCmd(void);
void ImDrawCmd_destroy(ImDrawCmd* self);
ImTextureID ImDrawCmd_GetTexID(ImDrawCmd* self);
ImDrawListSplitter* ImDrawListSplitter_ImDrawListSplitter(void);
void ImDrawListSplitter_destroy(ImDrawListSplitter* self);
void ImDrawListSplitter_Clear(ImDrawListSplitter* self);
void ImDrawListSplitter_ClearFreeMemory(ImDrawListSplitter* self);
void ImDrawListSplitter_Split(ImDrawListSplitter* self,ImDrawList* draw_list,int count);
void ImDrawListSplitter_Merge(ImDrawListSplitter* self,ImDrawList* draw_list);
void ImDrawListSplitter_SetCurrentChannel(ImDrawListSplitter* self,ImDrawList* draw_list,int channel_idx);
ImDrawList* ImDrawList_ImDrawList(ImDrawListSharedData* shared_data);
void ImDrawList_destroy(ImDrawList* self);
void ImDrawList_PushClipRect(ImDrawList* self,const ImVec2 clip_rect_min,const ImVec2 clip_rect_max,                                                                                                              _Bool                                                                                                                    intersect_with_current_clip_rect);
void ImDrawList_PushClipRectFullScreen(ImDrawList* self);
void ImDrawList_PopClipRect(ImDrawList* self);
void ImDrawList_PushTextureID(ImDrawList* self,ImTextureID texture_id);
void ImDrawList_PopTextureID(ImDrawList* self);
void ImDrawList_GetClipRectMin(ImVec2 *pOut,ImDrawList* self);
void ImDrawList_GetClipRectMax(ImVec2 *pOut,ImDrawList* self);
void ImDrawList_AddLine(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,ImU32 col,float thickness);
void ImDrawList_AddRect(ImDrawList* self,const ImVec2 p_min,const ImVec2 p_max,ImU32 col,float rounding,ImDrawFlags flags,float thickness);
void ImDrawList_AddRectFilled(ImDrawList* self,const ImVec2 p_min,const ImVec2 p_max,ImU32 col,float rounding,ImDrawFlags flags);
void ImDrawList_AddRectFilledMultiColor(ImDrawList* self,const ImVec2 p_min,const ImVec2 p_max,ImU32 col_upr_left,ImU32 col_upr_right,ImU32 col_bot_right,ImU32 col_bot_left);
void ImDrawList_AddQuad(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,ImU32 col,float thickness);
void ImDrawList_AddQuadFilled(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,ImU32 col);
void ImDrawList_AddTriangle(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,ImU32 col,float thickness);
void ImDrawList_AddTriangleFilled(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,ImU32 col);
void ImDrawList_AddCircle(ImDrawList* self,const ImVec2 center,float radius,ImU32 col,int num_segments,float thickness);
void ImDrawList_AddCircleFilled(ImDrawList* self,const ImVec2 center,float radius,ImU32 col,int num_segments);
void ImDrawList_AddNgon(ImDrawList* self,const ImVec2 center,float radius,ImU32 col,int num_segments,float thickness);
void ImDrawList_AddNgonFilled(ImDrawList* self,const ImVec2 center,float radius,ImU32 col,int num_segments);
void ImDrawList_AddEllipse(ImDrawList* self,const ImVec2 center,const ImVec2 radius,ImU32 col,float rot,int num_segments,float thickness);
void ImDrawList_AddEllipseFilled(ImDrawList* self,const ImVec2 center,const ImVec2 radius,ImU32 col,float rot,int num_segments);
void ImDrawList_AddText_Vec2(ImDrawList* self,const ImVec2 pos,ImU32 col,const char* text_begin,const char* text_end);
void ImDrawList_AddText_FontPtr(ImDrawList* self,const ImFont* font,float font_size,const ImVec2 pos,ImU32 col,const char* text_begin,const char* text_end,float wrap_width,const ImVec4* cpu_fine_clip_rect);
void ImDrawList_AddBezierCubic(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,ImU32 col,float thickness,int num_segments);
void ImDrawList_AddBezierQuadratic(ImDrawList* self,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,ImU32 col,float thickness,int num_segments);
void ImDrawList_AddPolyline(ImDrawList* self,const ImVec2* points,int num_points,ImU32 col,ImDrawFlags flags,float thickness);
void ImDrawList_AddConvexPolyFilled(ImDrawList* self,const ImVec2* points,int num_points,ImU32 col);
void ImDrawList_AddConcavePolyFilled(ImDrawList* self,const ImVec2* points,int num_points,ImU32 col);
void ImDrawList_AddImage(ImDrawList* self,ImTextureID user_texture_id,const ImVec2 p_min,const ImVec2 p_max,const ImVec2 uv_min,const ImVec2 uv_max,ImU32 col);
void ImDrawList_AddImageQuad(ImDrawList* self,ImTextureID user_texture_id,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,const ImVec2 uv1,const ImVec2 uv2,const ImVec2 uv3,const ImVec2 uv4,ImU32 col);
void ImDrawList_AddImageRounded(ImDrawList* self,ImTextureID user_texture_id,const ImVec2 p_min,const ImVec2 p_max,const ImVec2 uv_min,const ImVec2 uv_max,ImU32 col,float rounding,ImDrawFlags flags);
void ImDrawList_PathClear(ImDrawList* self);
void ImDrawList_PathLineTo(ImDrawList* self,const ImVec2 pos);
void ImDrawList_PathLineToMergeDuplicate(ImDrawList* self,const ImVec2 pos);
void ImDrawList_PathFillConvex(ImDrawList* self,ImU32 col);
void ImDrawList_PathFillConcave(ImDrawList* self,ImU32 col);
void ImDrawList_PathStroke(ImDrawList* self,ImU32 col,ImDrawFlags flags,float thickness);
void ImDrawList_PathArcTo(ImDrawList* self,const ImVec2 center,float radius,float a_min,float a_max,int num_segments);
void ImDrawList_PathArcToFast(ImDrawList* self,const ImVec2 center,float radius,int a_min_of_12,int a_max_of_12);
void ImDrawList_PathEllipticalArcTo(ImDrawList* self,const ImVec2 center,const ImVec2 radius,float rot,float a_min,float a_max,int num_segments);
void ImDrawList_PathBezierCubicCurveTo(ImDrawList* self,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,int num_segments);
void ImDrawList_PathBezierQuadraticCurveTo(ImDrawList* self,const ImVec2 p2,const ImVec2 p3,int num_segments);
void ImDrawList_PathRect(ImDrawList* self,const ImVec2 rect_min,const ImVec2 rect_max,float rounding,ImDrawFlags flags);
void ImDrawList_AddCallback(ImDrawList* self,ImDrawCallback callback,void* userdata,size_t userdata_size);
void ImDrawList_AddDrawCmd(ImDrawList* self);
ImDrawList* ImDrawList_CloneOutput(ImDrawList* self);
void ImDrawList_ChannelsSplit(ImDrawList* self,int count);
void ImDrawList_ChannelsMerge(ImDrawList* self);
void ImDrawList_ChannelsSetCurrent(ImDrawList* self,int n);
void ImDrawList_PrimReserve(ImDrawList* self,int idx_count,int vtx_count);
void ImDrawList_PrimUnreserve(ImDrawList* self,int idx_count,int vtx_count);
void ImDrawList_PrimRect(ImDrawList* self,const ImVec2 a,const ImVec2 b,ImU32 col);
void ImDrawList_PrimRectUV(ImDrawList* self,const ImVec2 a,const ImVec2 b,const ImVec2 uv_a,const ImVec2 uv_b,ImU32 col);
void ImDrawList_PrimQuadUV(ImDrawList* self,const ImVec2 a,const ImVec2 b,const ImVec2 c,const ImVec2 d,const ImVec2 uv_a,const ImVec2 uv_b,const ImVec2 uv_c,const ImVec2 uv_d,ImU32 col);
void ImDrawList_PrimWriteVtx(ImDrawList* self,const ImVec2 pos,const ImVec2 uv,ImU32 col);
void ImDrawList_PrimWriteIdx(ImDrawList* self,ImDrawIdx idx);
void ImDrawList_PrimVtx(ImDrawList* self,const ImVec2 pos,const ImVec2 uv,ImU32 col);
void ImDrawList__ResetForNewFrame(ImDrawList* self);
void ImDrawList__ClearFreeMemory(ImDrawList* self);
void ImDrawList__PopUnusedDrawCmd(ImDrawList* self);
void ImDrawList__TryMergeDrawCmds(ImDrawList* self);
void ImDrawList__OnChangedClipRect(ImDrawList* self);
void ImDrawList__OnChangedTextureID(ImDrawList* self);
void ImDrawList__OnChangedVtxOffset(ImDrawList* self);
void ImDrawList__SetTextureID(ImDrawList* self,ImTextureID texture_id);
int ImDrawList__CalcCircleAutoSegmentCount(ImDrawList* self,float radius);
void ImDrawList__PathArcToFastEx(ImDrawList* self,const ImVec2 center,float radius,int a_min_sample,int a_max_sample,int a_step);
void ImDrawList__PathArcToN(ImDrawList* self,const ImVec2 center,float radius,float a_min,float a_max,int num_segments);
ImDrawData* ImDrawData_ImDrawData(void);
void ImDrawData_destroy(ImDrawData* self);
void ImDrawData_Clear(ImDrawData* self);
void ImDrawData_AddDrawList(ImDrawData* self,ImDrawList* draw_list);
void ImDrawData_DeIndexAllBuffers(ImDrawData* self);
void ImDrawData_ScaleClipRects(ImDrawData* self,const ImVec2 fb_scale);
ImFontConfig* ImFontConfig_ImFontConfig(void);
void ImFontConfig_destroy(ImFontConfig* self);
ImFontGlyphRangesBuilder* ImFontGlyphRangesBuilder_ImFontGlyphRangesBuilder(void);
void ImFontGlyphRangesBuilder_destroy(ImFontGlyphRangesBuilder* self);
void ImFontGlyphRangesBuilder_Clear(ImFontGlyphRangesBuilder* self);
_Bool                ImFontGlyphRangesBuilder_GetBit(ImFontGlyphRangesBuilder* self,size_t n);
void ImFontGlyphRangesBuilder_SetBit(ImFontGlyphRangesBuilder* self,size_t n);
void ImFontGlyphRangesBuilder_AddChar(ImFontGlyphRangesBuilder* self,ImWchar c);
void ImFontGlyphRangesBuilder_AddText(ImFontGlyphRangesBuilder* self,const char* text,const char* text_end);
void ImFontGlyphRangesBuilder_AddRanges(ImFontGlyphRangesBuilder* self,const ImWchar* ranges);
void ImFontGlyphRangesBuilder_BuildRanges(ImFontGlyphRangesBuilder* self,ImVector_ImWchar* out_ranges);
ImFontAtlasCustomRect* ImFontAtlasCustomRect_ImFontAtlasCustomRect(void);
void ImFontAtlasCustomRect_destroy(ImFontAtlasCustomRect* self);
_Bool                ImFontAtlasCustomRect_IsPacked(ImFontAtlasCustomRect* self);
ImFontAtlas* ImFontAtlas_ImFontAtlas(void);
void ImFontAtlas_destroy(ImFontAtlas* self);
ImFont* ImFontAtlas_AddFont(ImFontAtlas* self,const ImFontConfig* font_cfg);
ImFont* ImFontAtlas_AddFontDefault(ImFontAtlas* self,const ImFontConfig* font_cfg);
ImFont* ImFontAtlas_AddFontFromFileTTF(ImFontAtlas* self,const char* filename,float size_pixels,const ImFontConfig* font_cfg,const ImWchar* glyph_ranges);
ImFont* ImFontAtlas_AddFontFromMemoryTTF(ImFontAtlas* self,void* font_data,int font_data_size,float size_pixels,const ImFontConfig* font_cfg,const ImWchar* glyph_ranges);
ImFont* ImFontAtlas_AddFontFromMemoryCompressedTTF(ImFontAtlas* self,const void* compressed_font_data,int compressed_font_data_size,float size_pixels,const ImFontConfig* font_cfg,const ImWchar* glyph_ranges);
ImFont* ImFontAtlas_AddFontFromMemoryCompressedBase85TTF(ImFontAtlas* self,const char* compressed_font_data_base85,float size_pixels,const ImFontConfig* font_cfg,const ImWchar* glyph_ranges);
void ImFontAtlas_ClearInputData(ImFontAtlas* self);
void ImFontAtlas_ClearTexData(ImFontAtlas* self);
void ImFontAtlas_ClearFonts(ImFontAtlas* self);
void ImFontAtlas_Clear(ImFontAtlas* self);
_Bool                ImFontAtlas_Build(ImFontAtlas* self);
void ImFontAtlas_GetTexDataAsAlpha8(ImFontAtlas* self,unsigned char** out_pixels,int* out_width,int* out_height,int* out_bytes_per_pixel);
void ImFontAtlas_GetTexDataAsRGBA32(ImFontAtlas* self,unsigned char** out_pixels,int* out_width,int* out_height,int* out_bytes_per_pixel);
_Bool                ImFontAtlas_IsBuilt(ImFontAtlas* self);
void ImFontAtlas_SetTexID(ImFontAtlas* self,ImTextureID id);
const ImWchar* ImFontAtlas_GetGlyphRangesDefault(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesGreek(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesKorean(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesJapanese(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesChineseFull(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesChineseSimplifiedCommon(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesCyrillic(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesThai(ImFontAtlas* self);
const ImWchar* ImFontAtlas_GetGlyphRangesVietnamese(ImFontAtlas* self);
int ImFontAtlas_AddCustomRectRegular(ImFontAtlas* self,int width,int height);
int ImFontAtlas_AddCustomRectFontGlyph(ImFontAtlas* self,ImFont* font,ImWchar id,int width,int height,float advance_x,const ImVec2 offset);
ImFontAtlasCustomRect* ImFontAtlas_GetCustomRectByIndex(ImFontAtlas* self,int index);
void ImFontAtlas_CalcCustomRectUV(ImFontAtlas* self,const ImFontAtlasCustomRect* rect,ImVec2* out_uv_min,ImVec2* out_uv_max);
_Bool                ImFontAtlas_GetMouseCursorTexData(ImFontAtlas* self,ImGuiMouseCursor cursor,ImVec2* out_offset,ImVec2* out_size,ImVec2 out_uv_border[2],ImVec2 out_uv_fill[2]);
ImFont* ImFont_ImFont(void);
void ImFont_destroy(ImFont* self);
const ImFontGlyph* ImFont_FindGlyph(ImFont* self,ImWchar c);
const ImFontGlyph* ImFont_FindGlyphNoFallback(ImFont* self,ImWchar c);
float ImFont_GetCharAdvance(ImFont* self,ImWchar c);
_Bool                ImFont_IsLoaded(ImFont* self);
const char* ImFont_GetDebugName(ImFont* self);
void ImFont_CalcTextSizeA(ImVec2 *pOut,ImFont* self,float size,float max_width,float wrap_width,const char* text_begin,const char* text_end,const char** remaining);
const char* ImFont_CalcWordWrapPositionA(ImFont* self,float scale,const char* text,const char* text_end,float wrap_width);
void ImFont_RenderChar(ImFont* self,ImDrawList* draw_list,float size,const ImVec2 pos,ImU32 col,ImWchar c);
void ImFont_RenderText(ImFont* self,ImDrawList* draw_list,float size,const ImVec2 pos,ImU32 col,const ImVec4 clip_rect,const char* text_begin,const char* text_end,float wrap_width,                                                                                                                                                                                              _Bool                                                                                                                                                                                                    cpu_fine_clip);
void ImFont_BuildLookupTable(ImFont* self);
void ImFont_ClearOutputData(ImFont* self);
void ImFont_GrowIndex(ImFont* self,int new_size);
void ImFont_AddGlyph(ImFont* self,const ImFontConfig* src_cfg,ImWchar c,float x0,float y0,float x1,float y1,float u0,float v0,float u1,float v1,float advance_x);
void ImFont_AddRemapChar(ImFont* self,ImWchar dst,ImWchar src,                                                                        _Bool                                                                              overwrite_dst);
void ImFont_SetGlyphVisible(ImFont* self,ImWchar c,                                                             _Bool                                                                   visible);
_Bool                ImFont_IsGlyphRangeUnused(ImFont* self,unsigned int c_begin,unsigned int c_last);
ImGuiViewport* ImGuiViewport_ImGuiViewport(void);
void ImGuiViewport_destroy(ImGuiViewport* self);
void ImGuiViewport_GetCenter(ImVec2 *pOut,ImGuiViewport* self);
void ImGuiViewport_GetWorkCenter(ImVec2 *pOut,ImGuiViewport* self);
ImGuiPlatformIO* ImGuiPlatformIO_ImGuiPlatformIO(void);
void ImGuiPlatformIO_destroy(ImGuiPlatformIO* self);
ImGuiPlatformMonitor* ImGuiPlatformMonitor_ImGuiPlatformMonitor(void);
void ImGuiPlatformMonitor_destroy(ImGuiPlatformMonitor* self);
ImGuiPlatformImeData* ImGuiPlatformImeData_ImGuiPlatformImeData(void);
void ImGuiPlatformImeData_destroy(ImGuiPlatformImeData* self);
ImGuiID igImHashData(const void* data,size_t data_size,ImGuiID seed);
ImGuiID igImHashStr(const char* data,size_t data_size,ImGuiID seed);
void igImQsort(void* base,size_t count,size_t size_of_element,int(*compare_func)(void const*,void const*));
ImU32 igImAlphaBlendColors(ImU32 col_a,ImU32 col_b);
_Bool                igImIsPowerOfTwo_Int(int v);
_Bool                igImIsPowerOfTwo_U64(ImU64 v);
int igImUpperPowerOfTwo(int v);
int igImStricmp(const char* str1,const char* str2);
int igImStrnicmp(const char* str1,const char* str2,size_t count);
void igImStrncpy(char* dst,const char* src,size_t count);
char* igImStrdup(const char* str);
char* igImStrdupcpy(char* dst,size_t* p_dst_size,const char* str);
const char* igImStrchrRange(const char* str_begin,const char* str_end,char c);
const char* igImStreolRange(const char* str,const char* str_end);
const char* igImStristr(const char* haystack,const char* haystack_end,const char* needle,const char* needle_end);
void igImStrTrimBlanks(char* str);
const char* igImStrSkipBlank(const char* str);
int igImStrlenW(const ImWchar* str);
const char* igImStrbol(const char* buf_mid_line,const char* buf_begin);
char igImToUpper(char c);
_Bool                igImCharIsBlankA(char c);
_Bool                igImCharIsBlankW(unsigned int c);
_Bool                igImCharIsXdigitA(char c);
int igImFormatString(char* buf,size_t buf_size,const char* fmt,...);
int igImFormatStringV(char* buf,size_t buf_size,const char* fmt,va_list args);
void igImFormatStringToTempBuffer(const char** out_buf,const char** out_buf_end,const char* fmt,...);
void igImFormatStringToTempBufferV(const char** out_buf,const char** out_buf_end,const char* fmt,va_list args);
const char* igImParseFormatFindStart(const char* format);
const char* igImParseFormatFindEnd(const char* format);
const char* igImParseFormatTrimDecorations(const char* format,char* buf,size_t buf_size);
void igImParseFormatSanitizeForPrinting(const char* fmt_in,char* fmt_out,size_t fmt_out_size);
const char* igImParseFormatSanitizeForScanning(const char* fmt_in,char* fmt_out,size_t fmt_out_size);
int igImParseFormatPrecision(const char* format,int default_value);
const char* igImTextCharToUtf8(char out_buf[5],unsigned int c);
int igImTextStrToUtf8(char* out_buf,int out_buf_size,const ImWchar* in_text,const ImWchar* in_text_end);
int igImTextCharFromUtf8(unsigned int* out_char,const char* in_text,const char* in_text_end);
int igImTextStrFromUtf8(ImWchar* out_buf,int out_buf_size,const char* in_text,const char* in_text_end,const char** in_remaining);
int igImTextCountCharsFromUtf8(const char* in_text,const char* in_text_end);
int igImTextCountUtf8BytesFromChar(const char* in_text,const char* in_text_end);
int igImTextCountUtf8BytesFromStr(const ImWchar* in_text,const ImWchar* in_text_end);
const char* igImTextFindPreviousUtf8Codepoint(const char* in_text_start,const char* in_text_curr);
int igImTextCountLines(const char* in_text,const char* in_text_end);
ImFileHandle igImFileOpen(const char* filename,const char* mode);
_Bool                igImFileClose(ImFileHandle file);
ImU64 igImFileGetSize(ImFileHandle file);
ImU64 igImFileRead(void* data,ImU64 size,ImU64 count,ImFileHandle file);
ImU64 igImFileWrite(const void* data,ImU64 size,ImU64 count,ImFileHandle file);
void* igImFileLoadToMemory(const char* filename,const char* mode,size_t* out_file_size,int padding_bytes);
float igImPow_Float(float x,float y);
double igImPow_double(double x,double y);
float igImLog_Float(float x);
double igImLog_double(double x);
int igImAbs_Int(int x);
float igImAbs_Float(float x);
double igImAbs_double(double x);
float igImSign_Float(float x);
double igImSign_double(double x);
float igImRsqrt_Float(float x);
double igImRsqrt_double(double x);
void igImMin(ImVec2 *pOut,const ImVec2 lhs,const ImVec2 rhs);
void igImMax(ImVec2 *pOut,const ImVec2 lhs,const ImVec2 rhs);
void igImClamp(ImVec2 *pOut,const ImVec2 v,const ImVec2 mn,const ImVec2 mx);
void igImLerp_Vec2Float(ImVec2 *pOut,const ImVec2 a,const ImVec2 b,float t);
void igImLerp_Vec2Vec2(ImVec2 *pOut,const ImVec2 a,const ImVec2 b,const ImVec2 t);
void igImLerp_Vec4(ImVec4 *pOut,const ImVec4 a,const ImVec4 b,float t);
float igImSaturate(float f);
float igImLengthSqr_Vec2(const ImVec2 lhs);
float igImLengthSqr_Vec4(const ImVec4 lhs);
float igImInvLength(const ImVec2 lhs,float fail_value);
float igImTrunc_Float(float f);
void igImTrunc_Vec2(ImVec2 *pOut,const ImVec2 v);
float igImFloor_Float(float f);
void igImFloor_Vec2(ImVec2 *pOut,const ImVec2 v);
int igImModPositive(int a,int b);
float igImDot(const ImVec2 a,const ImVec2 b);
void igImRotate(ImVec2 *pOut,const ImVec2 v,float cos_a,float sin_a);
float igImLinearSweep(float current,float target,float speed);
float igImLinearRemapClamp(float s0,float s1,float d0,float d1,float x);
void igImMul(ImVec2 *pOut,const ImVec2 lhs,const ImVec2 rhs);
_Bool                igImIsFloatAboveGuaranteedIntegerPrecision(float f);
float igImExponentialMovingAverage(float avg,float sample,int n);
void igImBezierCubicCalc(ImVec2 *pOut,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,float t);
void igImBezierCubicClosestPoint(ImVec2 *pOut,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,const ImVec2 p,int num_segments);
void igImBezierCubicClosestPointCasteljau(ImVec2 *pOut,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,const ImVec2 p4,const ImVec2 p,float tess_tol);
void igImBezierQuadraticCalc(ImVec2 *pOut,const ImVec2 p1,const ImVec2 p2,const ImVec2 p3,float t);
void igImLineClosestPoint(ImVec2 *pOut,const ImVec2 a,const ImVec2 b,const ImVec2 p);
_Bool                igImTriangleContainsPoint(const ImVec2 a,const ImVec2 b,const ImVec2 c,const ImVec2 p);
void igImTriangleClosestPoint(ImVec2 *pOut,const ImVec2 a,const ImVec2 b,const ImVec2 c,const ImVec2 p);
void igImTriangleBarycentricCoords(const ImVec2 a,const ImVec2 b,const ImVec2 c,const ImVec2 p,float* out_u,float* out_v,float* out_w);
float igImTriangleArea(const ImVec2 a,const ImVec2 b,const ImVec2 c);
_Bool                igImTriangleIsClockwise(const ImVec2 a,const ImVec2 b,const ImVec2 c);
ImVec1* ImVec1_ImVec1_Nil(void);
void ImVec1_destroy(ImVec1* self);
ImVec1* ImVec1_ImVec1_Float(float _x);
ImVec2ih* ImVec2ih_ImVec2ih_Nil(void);
void ImVec2ih_destroy(ImVec2ih* self);
ImVec2ih* ImVec2ih_ImVec2ih_short(short _x,short _y);
ImVec2ih* ImVec2ih_ImVec2ih_Vec2(const ImVec2 rhs);
ImRect* ImRect_ImRect_Nil(void);
void ImRect_destroy(ImRect* self);
ImRect* ImRect_ImRect_Vec2(const ImVec2 min,const ImVec2 max);
ImRect* ImRect_ImRect_Vec4(const ImVec4 v);
ImRect* ImRect_ImRect_Float(float x1,float y1,float x2,float y2);
void ImRect_GetCenter(ImVec2 *pOut,ImRect* self);
void ImRect_GetSize(ImVec2 *pOut,ImRect* self);
float ImRect_GetWidth(ImRect* self);
float ImRect_GetHeight(ImRect* self);
float ImRect_GetArea(ImRect* self);
void ImRect_GetTL(ImVec2 *pOut,ImRect* self);
void ImRect_GetTR(ImVec2 *pOut,ImRect* self);
void ImRect_GetBL(ImVec2 *pOut,ImRect* self);
void ImRect_GetBR(ImVec2 *pOut,ImRect* self);
_Bool                ImRect_Contains_Vec2(ImRect* self,const ImVec2 p);
_Bool                ImRect_Contains_Rect(ImRect* self,const ImRect r);
_Bool                ImRect_ContainsWithPad(ImRect* self,const ImVec2 p,const ImVec2 pad);
_Bool                ImRect_Overlaps(ImRect* self,const ImRect r);
void ImRect_Add_Vec2(ImRect* self,const ImVec2 p);
void ImRect_Add_Rect(ImRect* self,const ImRect r);
void ImRect_Expand_Float(ImRect* self,const float amount);
void ImRect_Expand_Vec2(ImRect* self,const ImVec2 amount);
void ImRect_Translate(ImRect* self,const ImVec2 d);
void ImRect_TranslateX(ImRect* self,float dx);
void ImRect_TranslateY(ImRect* self,float dy);
void ImRect_ClipWith(ImRect* self,const ImRect r);
void ImRect_ClipWithFull(ImRect* self,const ImRect r);
void ImRect_Floor(ImRect* self);
_Bool                ImRect_IsInverted(ImRect* self);
void ImRect_ToVec4(ImVec4 *pOut,ImRect* self);
size_t igImBitArrayGetStorageSizeInBytes(int bitcount);
void igImBitArrayClearAllBits(ImU32* arr,int bitcount);
_Bool                igImBitArrayTestBit(const ImU32* arr,int n);
void igImBitArrayClearBit(ImU32* arr,int n);
void igImBitArraySetBit(ImU32* arr,int n);
void igImBitArraySetBitRange(ImU32* arr,int n,int n2);
void ImBitVector_Create(ImBitVector* self,int sz);
void ImBitVector_Clear(ImBitVector* self);
_Bool                ImBitVector_TestBit(ImBitVector* self,int n);
void ImBitVector_SetBit(ImBitVector* self,int n);
void ImBitVector_ClearBit(ImBitVector* self,int n);
void ImGuiTextIndex_clear(ImGuiTextIndex* self);
int ImGuiTextIndex_size(ImGuiTextIndex* self);
const char* ImGuiTextIndex_get_line_begin(ImGuiTextIndex* self,const char* base,int n);
const char* ImGuiTextIndex_get_line_end(ImGuiTextIndex* self,const char* base,int n);
void ImGuiTextIndex_append(ImGuiTextIndex* self,const char* base,int old_size,int new_size);
ImGuiStoragePair* igImLowerBound(ImGuiStoragePair* in_begin,ImGuiStoragePair* in_end,ImGuiID key);
ImDrawListSharedData* ImDrawListSharedData_ImDrawListSharedData(void);
void ImDrawListSharedData_destroy(ImDrawListSharedData* self);
void ImDrawListSharedData_SetCircleTessellationMaxError(ImDrawListSharedData* self,float max_error);
ImDrawDataBuilder* ImDrawDataBuilder_ImDrawDataBuilder(void);
void ImDrawDataBuilder_destroy(ImDrawDataBuilder* self);
void* ImGuiDataVarInfo_GetVarPtr(ImGuiDataVarInfo* self,void* parent);
ImGuiStyleMod* ImGuiStyleMod_ImGuiStyleMod_Int(ImGuiStyleVar idx,int v);
void ImGuiStyleMod_destroy(ImGuiStyleMod* self);
ImGuiStyleMod* ImGuiStyleMod_ImGuiStyleMod_Float(ImGuiStyleVar idx,float v);
ImGuiStyleMod* ImGuiStyleMod_ImGuiStyleMod_Vec2(ImGuiStyleVar idx,ImVec2 v);
ImGuiComboPreviewData* ImGuiComboPreviewData_ImGuiComboPreviewData(void);
void ImGuiComboPreviewData_destroy(ImGuiComboPreviewData* self);
ImGuiMenuColumns* ImGuiMenuColumns_ImGuiMenuColumns(void);
void ImGuiMenuColumns_destroy(ImGuiMenuColumns* self);
void ImGuiMenuColumns_Update(ImGuiMenuColumns* self,float spacing,                                                                            _Bool                                                                                  window_reappearing);
float ImGuiMenuColumns_DeclColumns(ImGuiMenuColumns* self,float w_icon,float w_label,float w_shortcut,float w_mark);
void ImGuiMenuColumns_CalcNextTotalWidth(ImGuiMenuColumns* self,                                                                          _Bool                                                                                update_offsets);
ImGuiInputTextDeactivatedState* ImGuiInputTextDeactivatedState_ImGuiInputTextDeactivatedState(void);
void ImGuiInputTextDeactivatedState_destroy(ImGuiInputTextDeactivatedState* self);
void ImGuiInputTextDeactivatedState_ClearFreeMemory(ImGuiInputTextDeactivatedState* self);
ImGuiInputTextState* ImGuiInputTextState_ImGuiInputTextState(void);
void ImGuiInputTextState_destroy(ImGuiInputTextState* self);
void ImGuiInputTextState_ClearText(ImGuiInputTextState* self);
void ImGuiInputTextState_ClearFreeMemory(ImGuiInputTextState* self);
void ImGuiInputTextState_OnKeyPressed(ImGuiInputTextState* self,int key);
void ImGuiInputTextState_OnCharPressed(ImGuiInputTextState* self,unsigned int c);
void ImGuiInputTextState_CursorAnimReset(ImGuiInputTextState* self);
void ImGuiInputTextState_CursorClamp(ImGuiInputTextState* self);
_Bool                ImGuiInputTextState_HasSelection(ImGuiInputTextState* self);
void ImGuiInputTextState_ClearSelection(ImGuiInputTextState* self);
int ImGuiInputTextState_GetCursorPos(ImGuiInputTextState* self);
int ImGuiInputTextState_GetSelectionStart(ImGuiInputTextState* self);
int ImGuiInputTextState_GetSelectionEnd(ImGuiInputTextState* self);
void ImGuiInputTextState_SelectAll(ImGuiInputTextState* self);
void ImGuiInputTextState_ReloadUserBufAndSelectAll(ImGuiInputTextState* self);
void ImGuiInputTextState_ReloadUserBufAndKeepSelection(ImGuiInputTextState* self);
void ImGuiInputTextState_ReloadUserBufAndMoveToEnd(ImGuiInputTextState* self);
ImGuiNextWindowData* ImGuiNextWindowData_ImGuiNextWindowData(void);
void ImGuiNextWindowData_destroy(ImGuiNextWindowData* self);
void ImGuiNextWindowData_ClearFlags(ImGuiNextWindowData* self);
ImGuiNextItemData* ImGuiNextItemData_ImGuiNextItemData(void);
void ImGuiNextItemData_destroy(ImGuiNextItemData* self);
void ImGuiNextItemData_ClearFlags(ImGuiNextItemData* self);
ImGuiLastItemData* ImGuiLastItemData_ImGuiLastItemData(void);
void ImGuiLastItemData_destroy(ImGuiLastItemData* self);
ImGuiErrorRecoveryState* ImGuiErrorRecoveryState_ImGuiErrorRecoveryState(void);
void ImGuiErrorRecoveryState_destroy(ImGuiErrorRecoveryState* self);
ImGuiPtrOrIndex* ImGuiPtrOrIndex_ImGuiPtrOrIndex_Ptr(void* ptr);
void ImGuiPtrOrIndex_destroy(ImGuiPtrOrIndex* self);
ImGuiPtrOrIndex* ImGuiPtrOrIndex_ImGuiPtrOrIndex_Int(int index);
ImGuiPopupData* ImGuiPopupData_ImGuiPopupData(void);
void ImGuiPopupData_destroy(ImGuiPopupData* self);
ImGuiInputEvent* ImGuiInputEvent_ImGuiInputEvent(void);
void ImGuiInputEvent_destroy(ImGuiInputEvent* self);
ImGuiKeyRoutingData* ImGuiKeyRoutingData_ImGuiKeyRoutingData(void);
void ImGuiKeyRoutingData_destroy(ImGuiKeyRoutingData* self);
ImGuiKeyRoutingTable* ImGuiKeyRoutingTable_ImGuiKeyRoutingTable(void);
void ImGuiKeyRoutingTable_destroy(ImGuiKeyRoutingTable* self);
void ImGuiKeyRoutingTable_Clear(ImGuiKeyRoutingTable* self);
ImGuiKeyOwnerData* ImGuiKeyOwnerData_ImGuiKeyOwnerData(void);
void ImGuiKeyOwnerData_destroy(ImGuiKeyOwnerData* self);
ImGuiListClipperRange ImGuiListClipperRange_FromIndices(int min,int max);
ImGuiListClipperRange ImGuiListClipperRange_FromPositions(float y1,float y2,int off_min,int off_max);
ImGuiListClipperData* ImGuiListClipperData_ImGuiListClipperData(void);
void ImGuiListClipperData_destroy(ImGuiListClipperData* self);
void ImGuiListClipperData_Reset(ImGuiListClipperData* self,ImGuiListClipper* clipper);
ImGuiNavItemData* ImGuiNavItemData_ImGuiNavItemData(void);
void ImGuiNavItemData_destroy(ImGuiNavItemData* self);
void ImGuiNavItemData_Clear(ImGuiNavItemData* self);
ImGuiTypingSelectState* ImGuiTypingSelectState_ImGuiTypingSelectState(void);
void ImGuiTypingSelectState_destroy(ImGuiTypingSelectState* self);
void ImGuiTypingSelectState_Clear(ImGuiTypingSelectState* self);
ImGuiOldColumnData* ImGuiOldColumnData_ImGuiOldColumnData(void);
void ImGuiOldColumnData_destroy(ImGuiOldColumnData* self);
ImGuiOldColumns* ImGuiOldColumns_ImGuiOldColumns(void);
void ImGuiOldColumns_destroy(ImGuiOldColumns* self);
ImGuiBoxSelectState* ImGuiBoxSelectState_ImGuiBoxSelectState(void);
void ImGuiBoxSelectState_destroy(ImGuiBoxSelectState* self);
ImGuiMultiSelectTempData* ImGuiMultiSelectTempData_ImGuiMultiSelectTempData(void);
void ImGuiMultiSelectTempData_destroy(ImGuiMultiSelectTempData* self);
void ImGuiMultiSelectTempData_Clear(ImGuiMultiSelectTempData* self);
void ImGuiMultiSelectTempData_ClearIO(ImGuiMultiSelectTempData* self);
ImGuiMultiSelectState* ImGuiMultiSelectState_ImGuiMultiSelectState(void);
void ImGuiMultiSelectState_destroy(ImGuiMultiSelectState* self);
ImGuiDockNode* ImGuiDockNode_ImGuiDockNode(ImGuiID id);
void ImGuiDockNode_destroy(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsRootNode(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsDockSpace(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsFloatingNode(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsCentralNode(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsHiddenTabBar(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsNoTabBar(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsSplitNode(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsLeafNode(ImGuiDockNode* self);
_Bool                ImGuiDockNode_IsEmpty(ImGuiDockNode* self);
void ImGuiDockNode_Rect(ImRect *pOut,ImGuiDockNode* self);
void ImGuiDockNode_SetLocalFlags(ImGuiDockNode* self,ImGuiDockNodeFlags flags);
void ImGuiDockNode_UpdateMergedFlags(ImGuiDockNode* self);
ImGuiDockContext* ImGuiDockContext_ImGuiDockContext(void);
void ImGuiDockContext_destroy(ImGuiDockContext* self);
ImGuiViewportP* ImGuiViewportP_ImGuiViewportP(void);
void ImGuiViewportP_destroy(ImGuiViewportP* self);
void ImGuiViewportP_ClearRequestFlags(ImGuiViewportP* self);
void ImGuiViewportP_CalcWorkRectPos(ImVec2 *pOut,ImGuiViewportP* self,const ImVec2 inset_min);
void ImGuiViewportP_CalcWorkRectSize(ImVec2 *pOut,ImGuiViewportP* self,const ImVec2 inset_min,const ImVec2 inset_max);
void ImGuiViewportP_UpdateWorkRect(ImGuiViewportP* self);
void ImGuiViewportP_GetMainRect(ImRect *pOut,ImGuiViewportP* self);
void ImGuiViewportP_GetWorkRect(ImRect *pOut,ImGuiViewportP* self);
void ImGuiViewportP_GetBuildWorkRect(ImRect *pOut,ImGuiViewportP* self);
ImGuiWindowSettings* ImGuiWindowSettings_ImGuiWindowSettings(void);
void ImGuiWindowSettings_destroy(ImGuiWindowSettings* self);
char* ImGuiWindowSettings_GetName(ImGuiWindowSettings* self);
ImGuiSettingsHandler* ImGuiSettingsHandler_ImGuiSettingsHandler(void);
void ImGuiSettingsHandler_destroy(ImGuiSettingsHandler* self);
ImGuiDebugAllocInfo* ImGuiDebugAllocInfo_ImGuiDebugAllocInfo(void);
void ImGuiDebugAllocInfo_destroy(ImGuiDebugAllocInfo* self);
ImGuiStackLevelInfo* ImGuiStackLevelInfo_ImGuiStackLevelInfo(void);
void ImGuiStackLevelInfo_destroy(ImGuiStackLevelInfo* self);
ImGuiIDStackTool* ImGuiIDStackTool_ImGuiIDStackTool(void);
void ImGuiIDStackTool_destroy(ImGuiIDStackTool* self);
ImGuiContextHook* ImGuiContextHook_ImGuiContextHook(void);
void ImGuiContextHook_destroy(ImGuiContextHook* self);
ImGuiContext* ImGuiContext_ImGuiContext(ImFontAtlas* shared_font_atlas);
void ImGuiContext_destroy(ImGuiContext* self);
ImGuiWindow* ImGuiWindow_ImGuiWindow(ImGuiContext* context,const char* name);
void ImGuiWindow_destroy(ImGuiWindow* self);
ImGuiID ImGuiWindow_GetID_Str(ImGuiWindow* self,const char* str,const char* str_end);
ImGuiID ImGuiWindow_GetID_Ptr(ImGuiWindow* self,const void* ptr);
ImGuiID ImGuiWindow_GetID_Int(ImGuiWindow* self,int n);
ImGuiID ImGuiWindow_GetIDFromPos(ImGuiWindow* self,const ImVec2 p_abs);
ImGuiID ImGuiWindow_GetIDFromRectangle(ImGuiWindow* self,const ImRect r_abs);
void ImGuiWindow_Rect(ImRect *pOut,ImGuiWindow* self);
float ImGuiWindow_CalcFontSize(ImGuiWindow* self);
void ImGuiWindow_TitleBarRect(ImRect *pOut,ImGuiWindow* self);
void ImGuiWindow_MenuBarRect(ImRect *pOut,ImGuiWindow* self);
ImGuiTabItem* ImGuiTabItem_ImGuiTabItem(void);
void ImGuiTabItem_destroy(ImGuiTabItem* self);
ImGuiTabBar* ImGuiTabBar_ImGuiTabBar(void);
void ImGuiTabBar_destroy(ImGuiTabBar* self);
ImGuiTableColumn* ImGuiTableColumn_ImGuiTableColumn(void);
void ImGuiTableColumn_destroy(ImGuiTableColumn* self);
ImGuiTableInstanceData* ImGuiTableInstanceData_ImGuiTableInstanceData(void);
void ImGuiTableInstanceData_destroy(ImGuiTableInstanceData* self);
ImGuiTable* ImGuiTable_ImGuiTable(void);
void ImGuiTable_destroy(ImGuiTable* self);
ImGuiTableTempData* ImGuiTableTempData_ImGuiTableTempData(void);
void ImGuiTableTempData_destroy(ImGuiTableTempData* self);
ImGuiTableColumnSettings* ImGuiTableColumnSettings_ImGuiTableColumnSettings(void);
void ImGuiTableColumnSettings_destroy(ImGuiTableColumnSettings* self);
ImGuiTableSettings* ImGuiTableSettings_ImGuiTableSettings(void);
void ImGuiTableSettings_destroy(ImGuiTableSettings* self);
ImGuiTableColumnSettings* ImGuiTableSettings_GetColumnSettings(ImGuiTableSettings* self);
ImGuiWindow* igGetCurrentWindowRead(void);
ImGuiWindow* igGetCurrentWindow(void);
ImGuiWindow* igFindWindowByID(ImGuiID id);
ImGuiWindow* igFindWindowByName(const char* name);
void igUpdateWindowParentAndRootLinks(ImGuiWindow* window,ImGuiWindowFlags flags,ImGuiWindow* parent_window);
void igUpdateWindowSkipRefresh(ImGuiWindow* window);
void igCalcWindowNextAutoFitSize(ImVec2 *pOut,ImGuiWindow* window);
_Bool                igIsWindowChildOf(ImGuiWindow* window,ImGuiWindow* potential_parent,                                                                                   _Bool                                                                                         popup_hierarchy,                                                                                                        _Bool                                                                                                              dock_hierarchy);
_Bool                igIsWindowWithinBeginStackOf(ImGuiWindow* window,ImGuiWindow* potential_parent);
_Bool                igIsWindowAbove(ImGuiWindow* potential_above,ImGuiWindow* potential_below);
_Bool                igIsWindowNavFocusable(ImGuiWindow* window);
void igSetWindowPos_WindowPtr(ImGuiWindow* window,const ImVec2 pos,ImGuiCond cond);
void igSetWindowSize_WindowPtr(ImGuiWindow* window,const ImVec2 size,ImGuiCond cond);
void igSetWindowCollapsed_WindowPtr(ImGuiWindow* window,                                                                  _Bool                                                                        collapsed,ImGuiCond cond);
void igSetWindowHitTestHole(ImGuiWindow* window,const ImVec2 pos,const ImVec2 size);
void igSetWindowHiddenAndSkipItemsForCurrentFrame(ImGuiWindow* window);
void igSetWindowParentWindowForFocusRoute(ImGuiWindow* window,ImGuiWindow* parent_window);
void igWindowRectAbsToRel(ImRect *pOut,ImGuiWindow* window,const ImRect r);
void igWindowRectRelToAbs(ImRect *pOut,ImGuiWindow* window,const ImRect r);
void igWindowPosAbsToRel(ImVec2 *pOut,ImGuiWindow* window,const ImVec2 p);
void igWindowPosRelToAbs(ImVec2 *pOut,ImGuiWindow* window,const ImVec2 p);
void igFocusWindow(ImGuiWindow* window,ImGuiFocusRequestFlags flags);
void igFocusTopMostWindowUnderOne(ImGuiWindow* under_this_window,ImGuiWindow* ignore_window,ImGuiViewport* filter_viewport,ImGuiFocusRequestFlags flags);
void igBringWindowToFocusFront(ImGuiWindow* window);
void igBringWindowToDisplayFront(ImGuiWindow* window);
void igBringWindowToDisplayBack(ImGuiWindow* window);
void igBringWindowToDisplayBehind(ImGuiWindow* window,ImGuiWindow* above_window);
int igFindWindowDisplayIndex(ImGuiWindow* window);
ImGuiWindow* igFindBottomMostVisibleWindowWithinBeginStack(ImGuiWindow* window);
void igSetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags flags);
void igSetCurrentFont(ImFont* font);
ImFont* igGetDefaultFont(void);
ImDrawList* igGetForegroundDrawList_WindowPtr(ImGuiWindow* window);
void igAddDrawListToDrawDataEx(ImDrawData* draw_data,ImVector_ImDrawListPtr* out_list,ImDrawList* draw_list);
void igInitialize(void);
void igShutdown(void);
void igUpdateInputEvents(                                   _Bool                                         trickle_fast_inputs);
void igUpdateHoveredWindowAndCaptureFlags(void);
void igFindHoveredWindowEx(const ImVec2 pos,                                                      _Bool                                                            find_first_and_in_any_viewport,ImGuiWindow** out_hovered_window,ImGuiWindow** out_hovered_window_under_moving_window);
void igStartMouseMovingWindow(ImGuiWindow* window);
void igStartMouseMovingWindowOrNode(ImGuiWindow* window,ImGuiDockNode* node,                                                                                      _Bool                                                                                            undock);
void igUpdateMouseMovingWindowNewFrame(void);
void igUpdateMouseMovingWindowEndFrame(void);
ImGuiID igAddContextHook(ImGuiContext* context,const ImGuiContextHook* hook);
void igRemoveContextHook(ImGuiContext* context,ImGuiID hook_to_remove);
void igCallContextHooks(ImGuiContext* context,ImGuiContextHookType type);
void igTranslateWindowsInViewport(ImGuiViewportP* viewport,const ImVec2 old_pos,const ImVec2 new_pos,const ImVec2 old_size,const ImVec2 new_size);
void igScaleWindowsInViewport(ImGuiViewportP* viewport,float scale);
void igDestroyPlatformWindow(ImGuiViewportP* viewport);
void igSetWindowViewport(ImGuiWindow* window,ImGuiViewportP* viewport);
void igSetCurrentViewport(ImGuiWindow* window,ImGuiViewportP* viewport);
const ImGuiPlatformMonitor* igGetViewportPlatformMonitor(ImGuiViewport* viewport);
ImGuiViewportP* igFindHoveredViewportFromPlatformWindowStack(const ImVec2 mouse_platform_pos);
void igMarkIniSettingsDirty_Nil(void);
void igMarkIniSettingsDirty_WindowPtr(ImGuiWindow* window);
void igClearIniSettings(void);
void igAddSettingsHandler(const ImGuiSettingsHandler* handler);
void igRemoveSettingsHandler(const char* type_name);
ImGuiSettingsHandler* igFindSettingsHandler(const char* type_name);
ImGuiWindowSettings* igCreateNewWindowSettings(const char* name);
ImGuiWindowSettings* igFindWindowSettingsByID(ImGuiID id);
ImGuiWindowSettings* igFindWindowSettingsByWindow(ImGuiWindow* window);
void igClearWindowSettings(const char* name);
void igLocalizeRegisterEntries(const ImGuiLocEntry* entries,int count);
const char* igLocalizeGetMsg(ImGuiLocKey key);
void igSetScrollX_WindowPtr(ImGuiWindow* window,float scroll_x);
void igSetScrollY_WindowPtr(ImGuiWindow* window,float scroll_y);
void igSetScrollFromPosX_WindowPtr(ImGuiWindow* window,float local_x,float center_x_ratio);
void igSetScrollFromPosY_WindowPtr(ImGuiWindow* window,float local_y,float center_y_ratio);
void igScrollToItem(ImGuiScrollFlags flags);
void igScrollToRect(ImGuiWindow* window,const ImRect rect,ImGuiScrollFlags flags);
void igScrollToRectEx(ImVec2 *pOut,ImGuiWindow* window,const ImRect rect,ImGuiScrollFlags flags);
void igScrollToBringRectIntoView(ImGuiWindow* window,const ImRect rect);
ImGuiItemStatusFlags igGetItemStatusFlags(void);
ImGuiItemFlags igGetItemFlags(void);
ImGuiID igGetActiveID(void);
ImGuiID igGetFocusID(void);
void igSetActiveID(ImGuiID id,ImGuiWindow* window);
void igSetFocusID(ImGuiID id,ImGuiWindow* window);
void igClearActiveID(void);
ImGuiID igGetHoveredID(void);
void igSetHoveredID(ImGuiID id);
void igKeepAliveID(ImGuiID id);
void igMarkItemEdited(ImGuiID id);
void igPushOverrideID(ImGuiID id);
ImGuiID igGetIDWithSeed_Str(const char* str_id_begin,const char* str_id_end,ImGuiID seed);
ImGuiID igGetIDWithSeed_Int(int n,ImGuiID seed);
void igItemSize_Vec2(const ImVec2 size,float text_baseline_y);
void igItemSize_Rect(const ImRect bb,float text_baseline_y);
_Bool                igItemAdd(const ImRect bb,ImGuiID id,const ImRect* nav_bb,ImGuiItemFlags extra_flags);
_Bool                igItemHoverable(const ImRect bb,ImGuiID id,ImGuiItemFlags item_flags);
_Bool                igIsWindowContentHoverable(ImGuiWindow* window,ImGuiHoveredFlags flags);
_Bool                igIsClippedEx(const ImRect bb,ImGuiID id);
void igSetLastItemData(ImGuiID item_id,ImGuiItemFlags in_flags,ImGuiItemStatusFlags status_flags,const ImRect item_rect);
void igCalcItemSize(ImVec2 *pOut,ImVec2 size,float default_w,float default_h);
float igCalcWrapWidthForPos(const ImVec2 pos,float wrap_pos_x);
void igPushMultiItemsWidths(int components,float width_full);
void igShrinkWidths(ImGuiShrinkWidthItem* items,int count,float width_excess);
const ImGuiDataVarInfo* igGetStyleVarInfo(ImGuiStyleVar idx);
void igBeginDisabledOverrideReenable(void);
void igEndDisabledOverrideReenable(void);
void igLogBegin(ImGuiLogType type,int auto_open_depth);
void igLogToBuffer(int auto_open_depth);
void igLogRenderedText(const ImVec2* ref_pos,const char* text,const char* text_end);
void igLogSetNextTextDecoration(const char* prefix,const char* suffix);
_Bool                igBeginChildEx(const char* name,ImGuiID id,const ImVec2 size_arg,ImGuiChildFlags child_flags,ImGuiWindowFlags window_flags);
_Bool                igBeginPopupEx(ImGuiID id,ImGuiWindowFlags extra_window_flags);
void igOpenPopupEx(ImGuiID id,ImGuiPopupFlags popup_flags);
void igClosePopupToLevel(int remaining,                                                 _Bool                                                       restore_focus_to_window_under_popup);
void igClosePopupsOverWindow(ImGuiWindow* ref_window,                                                               _Bool                                                                     restore_focus_to_window_under_popup);
void igClosePopupsExceptModals(void);
_Bool                igIsPopupOpen_ID(ImGuiID id,ImGuiPopupFlags popup_flags);
void igGetPopupAllowedExtentRect(ImRect *pOut,ImGuiWindow* window);
ImGuiWindow* igGetTopMostPopupModal(void);
ImGuiWindow* igGetTopMostAndVisiblePopupModal(void);
ImGuiWindow* igFindBlockingModal(ImGuiWindow* window);
void igFindBestWindowPosForPopup(ImVec2 *pOut,ImGuiWindow* window);
void igFindBestWindowPosForPopupEx(ImVec2 *pOut,const ImVec2 ref_pos,const ImVec2 size,ImGuiDir* last_dir,const ImRect r_outer,const ImRect r_avoid,ImGuiPopupPositionPolicy policy);
_Bool                igBeginTooltipEx(ImGuiTooltipFlags tooltip_flags,ImGuiWindowFlags extra_window_flags);
_Bool                igBeginTooltipHidden(void);
_Bool                igBeginViewportSideBar(const char* name,ImGuiViewport* viewport,ImGuiDir dir,float size,ImGuiWindowFlags window_flags);
_Bool                igBeginMenuEx(const char* label,const char* icon,                                                                _Bool                                                                      enabled);
_Bool                igMenuItemEx(const char* label,const char* icon,const char* shortcut,                                                                                    _Bool                                                                                          selected,                                                                                                  _Bool                                                                                                        enabled);
_Bool                igBeginComboPopup(ImGuiID popup_id,const ImRect bb,ImGuiComboFlags flags);
_Bool                igBeginComboPreview(void);
void igEndComboPreview(void);
void igNavInitWindow(ImGuiWindow* window,                                                   _Bool                                                         force_reinit);
void igNavInitRequestApplyResult(void);
_Bool                igNavMoveRequestButNoResultYet(void);
void igNavMoveRequestSubmit(ImGuiDir move_dir,ImGuiDir clip_dir,ImGuiNavMoveFlags move_flags,ImGuiScrollFlags scroll_flags);
void igNavMoveRequestForward(ImGuiDir move_dir,ImGuiDir clip_dir,ImGuiNavMoveFlags move_flags,ImGuiScrollFlags scroll_flags);
void igNavMoveRequestResolveWithLastItem(ImGuiNavItemData* result);
void igNavMoveRequestResolveWithPastTreeNode(ImGuiNavItemData* result,ImGuiTreeNodeStackData* tree_node_data);
void igNavMoveRequestCancel(void);
void igNavMoveRequestApplyResult(void);
void igNavMoveRequestTryWrapping(ImGuiWindow* window,ImGuiNavMoveFlags move_flags);
void igNavHighlightActivated(ImGuiID id);
void igNavClearPreferredPosForAxis(ImGuiAxis axis);
void igSetNavCursorVisibleAfterMove(void);
void igNavUpdateCurrentWindowIsScrollPushableX(void);
void igSetNavWindow(ImGuiWindow* window);
void igSetNavID(ImGuiID id,ImGuiNavLayer nav_layer,ImGuiID focus_scope_id,const ImRect rect_rel);
void igSetNavFocusScope(ImGuiID focus_scope_id);
void igFocusItem(void);
void igActivateItemByID(ImGuiID id);
_Bool                igIsNamedKey(ImGuiKey key);
_Bool                igIsNamedKeyOrMod(ImGuiKey key);
_Bool                igIsLegacyKey(ImGuiKey key);
_Bool                igIsKeyboardKey(ImGuiKey key);
_Bool                igIsGamepadKey(ImGuiKey key);
_Bool                igIsMouseKey(ImGuiKey key);
_Bool                igIsAliasKey(ImGuiKey key);
_Bool                igIsLRModKey(ImGuiKey key);
ImGuiKeyChord igFixupKeyChord(ImGuiKeyChord key_chord);
ImGuiKey igConvertSingleModFlagToKey(ImGuiKey key);
ImGuiKeyData* igGetKeyData_ContextPtr(ImGuiContext* ctx,ImGuiKey key);
ImGuiKeyData* igGetKeyData_Key(ImGuiKey key);
const char* igGetKeyChordName(ImGuiKeyChord key_chord);
ImGuiKey igMouseButtonToKey(ImGuiMouseButton button);
_Bool                igIsMouseDragPastThreshold(ImGuiMouseButton button,float lock_threshold);
void igGetKeyMagnitude2d(ImVec2 *pOut,ImGuiKey key_left,ImGuiKey key_right,ImGuiKey key_up,ImGuiKey key_down);
float igGetNavTweakPressedAmount(ImGuiAxis axis);
int igCalcTypematicRepeatAmount(float t0,float t1,float repeat_delay,float repeat_rate);
void igGetTypematicRepeatRate(ImGuiInputFlags flags,float* repeat_delay,float* repeat_rate);
void igTeleportMousePos(const ImVec2 pos);
void igSetActiveIdUsingAllKeyboardKeys(void);
_Bool                igIsActiveIdUsingNavDir(ImGuiDir dir);
ImGuiID igGetKeyOwner(ImGuiKey key);
void igSetKeyOwner(ImGuiKey key,ImGuiID owner_id,ImGuiInputFlags flags);
void igSetKeyOwnersForKeyChord(ImGuiKeyChord key,ImGuiID owner_id,ImGuiInputFlags flags);
void igSetItemKeyOwner_InputFlags(ImGuiKey key,ImGuiInputFlags flags);
_Bool                igTestKeyOwner(ImGuiKey key,ImGuiID owner_id);
ImGuiKeyOwnerData* igGetKeyOwnerData(ImGuiContext* ctx,ImGuiKey key);
_Bool                igIsKeyDown_ID(ImGuiKey key,ImGuiID owner_id);
_Bool                igIsKeyPressed_InputFlags(ImGuiKey key,ImGuiInputFlags flags,ImGuiID owner_id);
_Bool                igIsKeyReleased_ID(ImGuiKey key,ImGuiID owner_id);
_Bool                igIsKeyChordPressed_InputFlags(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id);
_Bool                igIsMouseDown_ID(ImGuiMouseButton button,ImGuiID owner_id);
_Bool                igIsMouseClicked_InputFlags(ImGuiMouseButton button,ImGuiInputFlags flags,ImGuiID owner_id);
_Bool                igIsMouseReleased_ID(ImGuiMouseButton button,ImGuiID owner_id);
_Bool                igIsMouseDoubleClicked_ID(ImGuiMouseButton button,ImGuiID owner_id);
_Bool                igShortcut_ID(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id);
_Bool                igSetShortcutRouting(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id);
_Bool                igTestShortcutRouting(ImGuiKeyChord key_chord,ImGuiID owner_id);
ImGuiKeyRoutingData* igGetShortcutRoutingData(ImGuiKeyChord key_chord);
void igDockContextInitialize(ImGuiContext* ctx);
void igDockContextShutdown(ImGuiContext* ctx);
void igDockContextClearNodes(ImGuiContext* ctx,ImGuiID root_id,                                                                         _Bool                                                                               clear_settings_refs);
void igDockContextRebuildNodes(ImGuiContext* ctx);
void igDockContextNewFrameUpdateUndocking(ImGuiContext* ctx);
void igDockContextNewFrameUpdateDocking(ImGuiContext* ctx);
void igDockContextEndFrame(ImGuiContext* ctx);
ImGuiID igDockContextGenNodeID(ImGuiContext* ctx);
void igDockContextQueueDock(ImGuiContext* ctx,ImGuiWindow* target,ImGuiDockNode* target_node,ImGuiWindow* payload,ImGuiDir split_dir,float split_ratio,                                                                                                                                                                 _Bool                                                                                                                                                                       split_outer);
void igDockContextQueueUndockWindow(ImGuiContext* ctx,ImGuiWindow* window);
void igDockContextQueueUndockNode(ImGuiContext* ctx,ImGuiDockNode* node);
void igDockContextProcessUndockWindow(ImGuiContext* ctx,ImGuiWindow* window,                                                                                      _Bool                                                                                            clear_persistent_docking_ref);
void igDockContextProcessUndockNode(ImGuiContext* ctx,ImGuiDockNode* node);
_Bool                igDockContextCalcDropPosForDocking(ImGuiWindow* target,ImGuiDockNode* target_node,ImGuiWindow* payload_window,ImGuiDockNode* payload_node,ImGuiDir split_dir,                                                                                                                                                                            _Bool                                                                                                                                                                                  split_outer,ImVec2* out_pos);
ImGuiDockNode* igDockContextFindNodeByID(ImGuiContext* ctx,ImGuiID id);
void igDockNodeWindowMenuHandler_Default(ImGuiContext* ctx,ImGuiDockNode* node,ImGuiTabBar* tab_bar);
_Bool                igDockNodeBeginAmendTabBar(ImGuiDockNode* node);
void igDockNodeEndAmendTabBar(void);
ImGuiDockNode* igDockNodeGetRootNode(ImGuiDockNode* node);
_Bool                igDockNodeIsInHierarchyOf(ImGuiDockNode* node,ImGuiDockNode* parent);
int igDockNodeGetDepth(const ImGuiDockNode* node);
ImGuiID igDockNodeGetWindowMenuButtonId(const ImGuiDockNode* node);
ImGuiDockNode* igGetWindowDockNode(void);
_Bool                igGetWindowAlwaysWantOwnTabBar(ImGuiWindow* window);
void igBeginDocked(ImGuiWindow* window,                                                 _Bool                                                     * p_open);
void igBeginDockableDragDropSource(ImGuiWindow* window);
void igBeginDockableDragDropTarget(ImGuiWindow* window);
void igSetWindowDock(ImGuiWindow* window,ImGuiID dock_id,ImGuiCond cond);
void igDockBuilderDockWindow(const char* window_name,ImGuiID node_id);
ImGuiDockNode* igDockBuilderGetNode(ImGuiID node_id);
ImGuiDockNode* igDockBuilderGetCentralNode(ImGuiID node_id);
ImGuiID igDockBuilderAddNode(ImGuiID node_id,ImGuiDockNodeFlags flags);
void igDockBuilderRemoveNode(ImGuiID node_id);
void igDockBuilderRemoveNodeDockedWindows(ImGuiID node_id,                                                                    _Bool                                                                          clear_settings_refs);
void igDockBuilderRemoveNodeChildNodes(ImGuiID node_id);
void igDockBuilderSetNodePos(ImGuiID node_id,ImVec2 pos);
void igDockBuilderSetNodeSize(ImGuiID node_id,ImVec2 size);
ImGuiID igDockBuilderSplitNode(ImGuiID node_id,ImGuiDir split_dir,float size_ratio_for_node_at_dir,ImGuiID* out_id_at_dir,ImGuiID* out_id_at_opposite_dir);
void igDockBuilderCopyDockSpace(ImGuiID src_dockspace_id,ImGuiID dst_dockspace_id,ImVector_const_charPtr* in_window_remap_pairs);
void igDockBuilderCopyNode(ImGuiID src_node_id,ImGuiID dst_node_id,ImVector_ImGuiID* out_node_remap_pairs);
void igDockBuilderCopyWindowSettings(const char* src_name,const char* dst_name);
void igDockBuilderFinish(ImGuiID node_id);
void igPushFocusScope(ImGuiID id);
void igPopFocusScope(void);
ImGuiID igGetCurrentFocusScope(void);
_Bool                igIsDragDropActive(void);
_Bool                igBeginDragDropTargetCustom(const ImRect bb,ImGuiID id);
void igClearDragDrop(void);
_Bool                igIsDragDropPayloadBeingAccepted(void);
void igRenderDragDropTargetRect(const ImRect bb,const ImRect item_clip_rect);
ImGuiTypingSelectRequest* igGetTypingSelectRequest(ImGuiTypingSelectFlags flags);
int igTypingSelectFindMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data,int nav_item_idx);
int igTypingSelectFindNextSingleCharMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data,int nav_item_idx);
int igTypingSelectFindBestLeadingMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data);
_Bool                igBeginBoxSelect(const ImRect scope_rect,ImGuiWindow* window,ImGuiID box_select_id,ImGuiMultiSelectFlags ms_flags);
void igEndBoxSelect(const ImRect scope_rect,ImGuiMultiSelectFlags ms_flags);
void igMultiSelectItemHeader(ImGuiID id,                                                  _Bool                                                      * p_selected,ImGuiButtonFlags* p_button_flags);
void igMultiSelectItemFooter(ImGuiID id,                                                  _Bool                                                      * p_selected,                                                                   _Bool                                                                       * p_pressed);
void igMultiSelectAddSetAll(ImGuiMultiSelectTempData* ms,                                                                   _Bool                                                                         selected);
void igMultiSelectAddSetRange(ImGuiMultiSelectTempData* ms,                                                                     _Bool                                                                           selected,int range_dir,ImGuiSelectionUserData first_item,ImGuiSelectionUserData last_item);
ImGuiBoxSelectState* igGetBoxSelectState(ImGuiID id);
ImGuiMultiSelectState* igGetMultiSelectState(ImGuiID id);
void igSetWindowClipRectBeforeSetChannel(ImGuiWindow* window,const ImRect clip_rect);
void igBeginColumns(const char* str_id,int count,ImGuiOldColumnFlags flags);
void igEndColumns(void);
void igPushColumnClipRect(int column_index);
void igPushColumnsBackground(void);
void igPopColumnsBackground(void);
ImGuiID igGetColumnsID(const char* str_id,int count);
ImGuiOldColumns* igFindOrCreateColumns(ImGuiWindow* window,ImGuiID id);
float igGetColumnOffsetFromNorm(const ImGuiOldColumns* columns,float offset_norm);
float igGetColumnNormFromOffset(const ImGuiOldColumns* columns,float offset);
void igTableOpenContextMenu(int column_n);
void igTableSetColumnWidth(int column_n,float width);
void igTableSetColumnSortDirection(int column_n,ImGuiSortDirection sort_direction,                                                                                            _Bool                                                                                                  append_to_sort_specs);
int igTableGetHoveredRow(void);
float igTableGetHeaderRowHeight(void);
float igTableGetHeaderAngledMaxLabelWidth(void);
void igTablePushBackgroundChannel(void);
void igTablePopBackgroundChannel(void);
void igTableAngledHeadersRowEx(ImGuiID row_id,float angle,float max_label_width,const ImGuiTableHeaderData* data,int data_count);
ImGuiTable* igGetCurrentTable(void);
ImGuiTable* igTableFindByID(ImGuiID id);
_Bool                igBeginTableEx(const char* name,ImGuiID id,int columns_count,ImGuiTableFlags flags,const ImVec2 outer_size,float inner_width);
void igTableBeginInitMemory(ImGuiTable* table,int columns_count);
void igTableBeginApplyRequests(ImGuiTable* table);
void igTableSetupDrawChannels(ImGuiTable* table);
void igTableUpdateLayout(ImGuiTable* table);
void igTableUpdateBorders(ImGuiTable* table);
void igTableUpdateColumnsWeightFromWidth(ImGuiTable* table);
void igTableDrawBorders(ImGuiTable* table);
void igTableDrawDefaultContextMenu(ImGuiTable* table,ImGuiTableFlags flags_for_section_to_display);
_Bool                igTableBeginContextMenuPopup(ImGuiTable* table);
void igTableMergeDrawChannels(ImGuiTable* table);
ImGuiTableInstanceData* igTableGetInstanceData(ImGuiTable* table,int instance_no);
ImGuiID igTableGetInstanceID(ImGuiTable* table,int instance_no);
void igTableSortSpecsSanitize(ImGuiTable* table);
void igTableSortSpecsBuild(ImGuiTable* table);
ImGuiSortDirection igTableGetColumnNextSortDirection(ImGuiTableColumn* column);
void igTableFixColumnSortDirection(ImGuiTable* table,ImGuiTableColumn* column);
float igTableGetColumnWidthAuto(ImGuiTable* table,ImGuiTableColumn* column);
void igTableBeginRow(ImGuiTable* table);
void igTableEndRow(ImGuiTable* table);
void igTableBeginCell(ImGuiTable* table,int column_n);
void igTableEndCell(ImGuiTable* table);
void igTableGetCellBgRect(ImRect *pOut,const ImGuiTable* table,int column_n);
const char* igTableGetColumnName_TablePtr(const ImGuiTable* table,int column_n);
ImGuiID igTableGetColumnResizeID(ImGuiTable* table,int column_n,int instance_no);
float igTableCalcMaxColumnWidth(const ImGuiTable* table,int column_n);
void igTableSetColumnWidthAutoSingle(ImGuiTable* table,int column_n);
void igTableSetColumnWidthAutoAll(ImGuiTable* table);
void igTableRemove(ImGuiTable* table);
void igTableGcCompactTransientBuffers_TablePtr(ImGuiTable* table);
void igTableGcCompactTransientBuffers_TableTempDataPtr(ImGuiTableTempData* table);
void igTableGcCompactSettings(void);
void igTableLoadSettings(ImGuiTable* table);
void igTableSaveSettings(ImGuiTable* table);
void igTableResetSettings(ImGuiTable* table);
ImGuiTableSettings* igTableGetBoundSettings(ImGuiTable* table);
void igTableSettingsAddSettingsHandler(void);
ImGuiTableSettings* igTableSettingsCreate(ImGuiID id,int columns_count);
ImGuiTableSettings* igTableSettingsFindByID(ImGuiID id);
ImGuiTabBar* igGetCurrentTabBar(void);
_Bool                igBeginTabBarEx(ImGuiTabBar* tab_bar,const ImRect bb,ImGuiTabBarFlags flags);
ImGuiTabItem* igTabBarFindTabByID(ImGuiTabBar* tab_bar,ImGuiID tab_id);
ImGuiTabItem* igTabBarFindTabByOrder(ImGuiTabBar* tab_bar,int order);
ImGuiTabItem* igTabBarFindMostRecentlySelectedTabForActiveWindow(ImGuiTabBar* tab_bar);
ImGuiTabItem* igTabBarGetCurrentTab(ImGuiTabBar* tab_bar);
int igTabBarGetTabOrder(ImGuiTabBar* tab_bar,ImGuiTabItem* tab);
const char* igTabBarGetTabName(ImGuiTabBar* tab_bar,ImGuiTabItem* tab);
void igTabBarAddTab(ImGuiTabBar* tab_bar,ImGuiTabItemFlags tab_flags,ImGuiWindow* window);
void igTabBarRemoveTab(ImGuiTabBar* tab_bar,ImGuiID tab_id);
void igTabBarCloseTab(ImGuiTabBar* tab_bar,ImGuiTabItem* tab);
void igTabBarQueueFocus_TabItemPtr(ImGuiTabBar* tab_bar,ImGuiTabItem* tab);
void igTabBarQueueFocus_Str(ImGuiTabBar* tab_bar,const char* tab_name);
void igTabBarQueueReorder(ImGuiTabBar* tab_bar,ImGuiTabItem* tab,int offset);
void igTabBarQueueReorderFromMousePos(ImGuiTabBar* tab_bar,ImGuiTabItem* tab,ImVec2 mouse_pos);
_Bool                igTabBarProcessReorder(ImGuiTabBar* tab_bar);
_Bool                igTabItemEx(ImGuiTabBar* tab_bar,const char* label,                                                                  _Bool                                                                      * p_open,ImGuiTabItemFlags flags,ImGuiWindow* docked_window);
void igTabItemCalcSize_Str(ImVec2 *pOut,const char* label,                                                                    _Bool                                                                          has_close_button_or_unsaved_marker);
void igTabItemCalcSize_WindowPtr(ImVec2 *pOut,ImGuiWindow* window);
void igTabItemBackground(ImDrawList* draw_list,const ImRect bb,ImGuiTabItemFlags flags,ImU32 col);
void igTabItemLabelAndCloseButton(ImDrawList* draw_list,const ImRect bb,ImGuiTabItemFlags flags,ImVec2 frame_padding,const char* label,ImGuiID tab_id,ImGuiID close_button_id,                                                                                                                                                                                        _Bool                                                                                                                                                                                              is_contents_visible,                                                                                                                                                                                                                 _Bool                                                                                                                                                                                                                     * out_just_closed,                                                                                                                                                                                                                                       _Bool                                                                                                                                                                                                                                           * out_text_clipped);
void igRenderText(ImVec2 pos,const char* text,const char* text_end,                                                                             _Bool                                                                                   hide_text_after_hash);
void igRenderTextWrapped(ImVec2 pos,const char* text,const char* text_end,float wrap_width);
void igRenderTextClipped(const ImVec2 pos_min,const ImVec2 pos_max,const char* text,const char* text_end,const ImVec2* text_size_if_known,const ImVec2 align,const ImRect* clip_rect);
void igRenderTextClippedEx(ImDrawList* draw_list,const ImVec2 pos_min,const ImVec2 pos_max,const char* text,const char* text_end,const ImVec2* text_size_if_known,const ImVec2 align,const ImRect* clip_rect);
void igRenderTextEllipsis(ImDrawList* draw_list,const ImVec2 pos_min,const ImVec2 pos_max,float clip_max_x,float ellipsis_max_x,const char* text,const char* text_end,const ImVec2* text_size_if_known);
void igRenderFrame(ImVec2 p_min,ImVec2 p_max,ImU32 fill_col,                                                                      _Bool                                                                            borders,float rounding);
void igRenderFrameBorder(ImVec2 p_min,ImVec2 p_max,float rounding);
void igRenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list,ImVec2 p_min,ImVec2 p_max,ImU32 fill_col,float grid_step,ImVec2 grid_off,float rounding,ImDrawFlags flags);
void igRenderNavCursor(const ImRect bb,ImGuiID id,ImGuiNavRenderCursorFlags flags);
const char* igFindRenderedTextEnd(const char* text,const char* text_end);
void igRenderMouseCursor(ImVec2 pos,float scale,ImGuiMouseCursor mouse_cursor,ImU32 col_fill,ImU32 col_border,ImU32 col_shadow);
void igRenderArrow(ImDrawList* draw_list,ImVec2 pos,ImU32 col,ImGuiDir dir,float scale);
void igRenderBullet(ImDrawList* draw_list,ImVec2 pos,ImU32 col);
void igRenderCheckMark(ImDrawList* draw_list,ImVec2 pos,ImU32 col,float sz);
void igRenderArrowPointingAt(ImDrawList* draw_list,ImVec2 pos,ImVec2 half_sz,ImGuiDir direction,ImU32 col);
void igRenderArrowDockMenu(ImDrawList* draw_list,ImVec2 p_min,float sz,ImU32 col);
void igRenderRectFilledRangeH(ImDrawList* draw_list,const ImRect rect,ImU32 col,float x_start_norm,float x_end_norm,float rounding);
void igRenderRectFilledWithHole(ImDrawList* draw_list,const ImRect outer,const ImRect inner,ImU32 col,float rounding);
ImDrawFlags igCalcRoundingFlagsForRectInRect(const ImRect r_in,const ImRect r_outer,float threshold);
void igTextEx(const char* text,const char* text_end,ImGuiTextFlags flags);
_Bool                igButtonEx(const char* label,const ImVec2 size_arg,ImGuiButtonFlags flags);
_Bool                igArrowButtonEx(const char* str_id,ImGuiDir dir,ImVec2 size_arg,ImGuiButtonFlags flags);
_Bool                igImageButtonEx(ImGuiID id,ImTextureID texture_id,const ImVec2 image_size,const ImVec2 uv0,const ImVec2 uv1,const ImVec4 bg_col,const ImVec4 tint_col,ImGuiButtonFlags flags);
void igSeparatorEx(ImGuiSeparatorFlags flags,float thickness);
void igSeparatorTextEx(ImGuiID id,const char* label,const char* label_end,float extra_width);
_Bool                igCheckboxFlags_S64Ptr(const char* label,ImS64* flags,ImS64 flags_value);
_Bool                igCheckboxFlags_U64Ptr(const char* label,ImU64* flags,ImU64 flags_value);
_Bool                igCloseButton(ImGuiID id,const ImVec2 pos);
_Bool                igCollapseButton(ImGuiID id,const ImVec2 pos,ImGuiDockNode* dock_node);
void igScrollbar(ImGuiAxis axis);
_Bool                igScrollbarEx(const ImRect bb,ImGuiID id,ImGuiAxis axis,ImS64* p_scroll_v,ImS64 avail_v,ImS64 contents_v,ImDrawFlags flags);
void igGetWindowScrollbarRect(ImRect *pOut,ImGuiWindow* window,ImGuiAxis axis);
ImGuiID igGetWindowScrollbarID(ImGuiWindow* window,ImGuiAxis axis);
ImGuiID igGetWindowResizeCornerID(ImGuiWindow* window,int n);
ImGuiID igGetWindowResizeBorderID(ImGuiWindow* window,ImGuiDir dir);
_Bool                igButtonBehavior(const ImRect bb,ImGuiID id,                                                           _Bool                                                               * out_hovered,                                                                             _Bool                                                                                 * out_held,ImGuiButtonFlags flags);
_Bool                igDragBehavior(ImGuiID id,ImGuiDataType data_type,void* p_v,float v_speed,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags);
_Bool                igSliderBehavior(const ImRect bb,ImGuiID id,ImGuiDataType data_type,void* p_v,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags,ImRect* out_grab_bb);
_Bool                igSplitterBehavior(const ImRect bb,ImGuiID id,ImGuiAxis axis,float* size1,float* size2,float min_size1,float min_size2,float hover_extend,float hover_visibility_delay,ImU32 bg_col);
_Bool                igTreeNodeBehavior(ImGuiID id,ImGuiTreeNodeFlags flags,const char* label,const char* label_end);
void igTreePushOverrideID(ImGuiID id);
_Bool                igTreeNodeGetOpen(ImGuiID storage_id);
void igTreeNodeSetOpen(ImGuiID storage_id,                                                    _Bool                                                          open);
_Bool                igTreeNodeUpdateNextOpen(ImGuiID storage_id,ImGuiTreeNodeFlags flags);
const ImGuiDataTypeInfo* igDataTypeGetInfo(ImGuiDataType data_type);
int igDataTypeFormatString(char* buf,int buf_size,ImGuiDataType data_type,const void* p_data,const char* format);
void igDataTypeApplyOp(ImGuiDataType data_type,int op,void* output,const void* arg_1,const void* arg_2);
_Bool                igDataTypeApplyFromText(const char* buf,ImGuiDataType data_type,void* p_data,const char* format,void* p_data_when_empty);
int igDataTypeCompare(ImGuiDataType data_type,const void* arg_1,const void* arg_2);
_Bool                igDataTypeClamp(ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max);
_Bool                igDataTypeIsZero(ImGuiDataType data_type,const void* p_data);
_Bool                igInputTextEx(const char* label,const char* hint,char* buf,int buf_size,const ImVec2 size_arg,ImGuiInputTextFlags flags,ImGuiInputTextCallback callback,void* user_data);
void igInputTextDeactivateHook(ImGuiID id);
_Bool                igTempInputText(const ImRect bb,ImGuiID id,const char* label,char* buf,int buf_size,ImGuiInputTextFlags flags);
_Bool                igTempInputScalar(const ImRect bb,ImGuiID id,const char* label,ImGuiDataType data_type,void* p_data,const char* format,const void* p_clamp_min,const void* p_clamp_max);
_Bool                igTempInputIsActive(ImGuiID id);
ImGuiInputTextState* igGetInputTextState(ImGuiID id);
void igSetNextItemRefVal(ImGuiDataType data_type,void* p_data);
void igColorTooltip(const char* text,const float* col,ImGuiColorEditFlags flags);
void igColorEditOptionsPopup(const float* col,ImGuiColorEditFlags flags);
void igColorPickerOptionsPopup(const float* ref_col,ImGuiColorEditFlags flags);
int igPlotEx(ImGuiPlotType plot_type,const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,const ImVec2 size_arg);
void igShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,ImVec2 gradient_p0,ImVec2 gradient_p1,ImU32 col0,ImU32 col1);
void igShadeVertsLinearUV(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,const ImVec2 a,const ImVec2 b,const ImVec2 uv_a,const ImVec2 uv_b,                                                                                                                                                                _Bool                                                                                                                                                                      clamp);
void igShadeVertsTransformPos(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,const ImVec2 pivot_in,float cos_a,float sin_a,const ImVec2 pivot_out);
void igGcCompactTransientMiscBuffers(void);
void igGcCompactTransientWindowBuffers(ImGuiWindow* window);
void igGcAwakeTransientWindowBuffers(ImGuiWindow* window);
_Bool                igErrorLog(const char* msg);
void igErrorRecoveryStoreState(ImGuiErrorRecoveryState* state_out);
void igErrorRecoveryTryToRecoverState(const ImGuiErrorRecoveryState* state_in);
void igErrorRecoveryTryToRecoverWindowState(const ImGuiErrorRecoveryState* state_in);
void igErrorCheckUsingSetCursorPosToExtendParentBoundaries(void);
void igErrorCheckEndFrameFinalizeErrorTooltip(void);
_Bool                igBeginErrorTooltip(void);
void igEndErrorTooltip(void);
void igDebugAllocHook(ImGuiDebugAllocInfo* info,int frame_count,void* ptr,size_t size);
void igDebugDrawCursorPos(ImU32 col);
void igDebugDrawLineExtents(ImU32 col);
void igDebugDrawItemRect(ImU32 col);
void igDebugTextUnformattedWithLocateItem(const char* line_begin,const char* line_end);
void igDebugLocateItem(ImGuiID target_id);
void igDebugLocateItemOnHover(ImGuiID target_id);
void igDebugLocateItemResolveWithLastItem(void);
void igDebugBreakClearData(void);
_Bool                igDebugBreakButton(const char* label,const char* description_of_location);
void igDebugBreakButtonTooltip(                                         _Bool                                               keyboard_only,const char* description_of_location);
void igShowFontAtlas(ImFontAtlas* atlas);
void igDebugHookIdInfo(ImGuiID id,ImGuiDataType data_type,const void* data_id,const void* data_id_end);
void igDebugNodeColumns(ImGuiOldColumns* columns);
void igDebugNodeDockNode(ImGuiDockNode* node,const char* label);
void igDebugNodeDrawList(ImGuiWindow* window,ImGuiViewportP* viewport,const ImDrawList* draw_list,const char* label);
void igDebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list,const ImDrawList* draw_list,const ImDrawCmd* draw_cmd,                                                                                                                                        _Bool                                                                                                                                              show_mesh,                                                                                                                                                       _Bool                                                                                                                                                             show_aabb);
void igDebugNodeFont(ImFont* font);
void igDebugNodeFontGlyph(ImFont* font,const ImFontGlyph* glyph);
void igDebugNodeStorage(ImGuiStorage* storage,const char* label);
void igDebugNodeTabBar(ImGuiTabBar* tab_bar,const char* label);
void igDebugNodeTable(ImGuiTable* table);
void igDebugNodeTableSettings(ImGuiTableSettings* settings);
void igDebugNodeInputTextState(ImGuiInputTextState* state);
void igDebugNodeTypingSelectState(ImGuiTypingSelectState* state);
void igDebugNodeMultiSelectState(ImGuiMultiSelectState* state);
void igDebugNodeWindow(ImGuiWindow* window,const char* label);
void igDebugNodeWindowSettings(ImGuiWindowSettings* settings);
void igDebugNodeWindowsList(ImVector_ImGuiWindowPtr* windows,const char* label);
void igDebugNodeWindowsListByBeginStackParent(ImGuiWindow** windows,int windows_size,ImGuiWindow* parent_in_begin_stack);
void igDebugNodeViewport(ImGuiViewportP* viewport);
void igDebugNodePlatformMonitor(ImGuiPlatformMonitor* monitor,const char* label,int idx);
void igDebugRenderKeyboardPreview(ImDrawList* draw_list);
void igDebugRenderViewportThumbnail(ImDrawList* draw_list,ImGuiViewportP* viewport,const ImRect bb);
const ImFontBuilderIO* igImFontAtlasGetBuilderForStbTruetype(void);
void igImFontAtlasUpdateConfigDataPointers(ImFontAtlas* atlas);
void igImFontAtlasBuildInit(ImFontAtlas* atlas);
void igImFontAtlasBuildSetupFont(ImFontAtlas* atlas,ImFont* font,ImFontConfig* font_config,float ascent,float descent);
void igImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas,void* stbrp_context_opaque);
void igImFontAtlasBuildFinish(ImFontAtlas* atlas);
void igImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas,int x,int y,int w,int h,const char* in_str,char in_marker_char,unsigned char in_marker_pixel_value);
void igImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas,int x,int y,int w,int h,const char* in_str,char in_marker_char,unsigned int in_marker_pixel_value);
void igImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256],float in_multiply_factor);
void igImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256],unsigned char* pixels,int x,int y,int w,int h,int stride);
void igLogText(const char *fmt, ...);
void ImGuiTextBuffer_appendf(struct ImGuiTextBuffer *buffer, const char *fmt, ...);
float igGET_FLT_MAX(void);
float igGET_FLT_MIN(void);
ImVector_ImWchar* ImVector_ImWchar_create(void);
void ImVector_ImWchar_destroy(ImVector_ImWchar* self);
void ImVector_ImWchar_Init(ImVector_ImWchar* p);
void ImVector_ImWchar_UnInit(ImVector_ImWchar* p);
void ImGuiPlatformIO_Set_Platform_GetWindowPos(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_pos));
void ImGuiPlatformIO_Set_Platform_GetWindowSize(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_size));
typedef struct tm tm;
typedef struct ImVector_ImS16 {int Size;int Capacity;ImS16* Data;} ImVector_ImS16;
typedef struct ImVector_ImS32 {int Size;int Capacity;ImS32* Data;} ImVector_ImS32;
typedef struct ImVector_ImS64 {int Size;int Capacity;ImS64* Data;} ImVector_ImS64;
typedef struct ImVector_ImS8 {int Size;int Capacity;ImS8* Data;} ImVector_ImS8;
typedef struct ImVector_ImU16 {int Size;int Capacity;ImU16* Data;} ImVector_ImU16;
typedef struct ImVector_ImU64 {int Size;int Capacity;ImU64* Data;} ImVector_ImU64;

typedef struct ImGuiWindow ImGuiWindow;
struct ImGuiWindow;
typedef struct ImGuiContext ImGuiContext;
typedef struct ImVec2 ImVec2;




typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWmonitor GLFWmonitor;
struct GLFWwindow;
struct GLFWmonitor; bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window,bool install_callbacks);
 bool ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window,bool install_callbacks);
 bool ImGui_ImplGlfw_InitForOther(GLFWwindow* window,bool install_callbacks);
 void ImGui_ImplGlfw_Shutdown(void);
 void ImGui_ImplGlfw_NewFrame(void);
 void ImGui_ImplGlfw_InstallCallbacks(GLFWwindow* window);
 void ImGui_ImplGlfw_RestoreCallbacks(GLFWwindow* window);
 void ImGui_ImplGlfw_SetCallbacksChainForAllWindows(bool chain_for_all_windows);
 void ImGui_ImplGlfw_WindowFocusCallback(GLFWwindow* window,int focused);
 void ImGui_ImplGlfw_CursorEnterCallback(GLFWwindow* window,int entered);
 void ImGui_ImplGlfw_CursorPosCallback(GLFWwindow* window,double x,double y);
 void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window,int button,int action,int mods);
 void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window,double xoffset,double yoffset);
 void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window,int key,int scancode,int action,int mods);
 void ImGui_ImplGlfw_CharCallback(GLFWwindow* window,unsigned int c);
 void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor* monitor,int event);
 void ImGui_ImplGlfw_Sleep(int milliseconds);
 bool ImGui_ImplOpenGL3_Init(const char* glsl_version);
 void ImGui_ImplOpenGL3_Shutdown(void);
 void ImGui_ImplOpenGL3_NewFrame(void);
 void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);
 bool ImGui_ImplOpenGL3_CreateFontsTexture(void);
 void ImGui_ImplOpenGL3_DestroyFontsTexture(void);
 bool ImGui_ImplOpenGL3_CreateDeviceObjects(void);
 void ImGui_ImplOpenGL3_DestroyDeviceObjects(void);
 bool ImGui_ImplOpenGL2_Init(void);
 void ImGui_ImplOpenGL2_Shutdown(void);
 void ImGui_ImplOpenGL2_NewFrame(void);
 void ImGui_ImplOpenGL2_RenderDrawData(ImDrawData* draw_data);
 bool ImGui_ImplOpenGL2_CreateFontsTexture(void);
 void ImGui_ImplOpenGL2_DestroyFontsTexture(void);
 bool ImGui_ImplOpenGL2_CreateDeviceObjects(void);
 void ImGui_ImplOpenGL2_DestroyDeviceObjects(void);
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct _SDL_GameController _SDL_GameController;
struct SDL_Window;
struct SDL_Renderer;
struct _SDL_GameController;
typedef union SDL_Event SDL_Event;
typedef enum { ImGui_ImplSDL2_GamepadMode_AutoFirst, ImGui_ImplSDL2_GamepadMode_AutoAll, ImGui_ImplSDL2_GamepadMode_Manual }ImGui_ImplSDL2_GamepadMode; bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window,void* sdl_gl_context);
 bool ImGui_ImplSDL2_InitForVulkan(SDL_Window* window);
 bool ImGui_ImplSDL2_InitForD3D(SDL_Window* window);
 bool ImGui_ImplSDL2_InitForMetal(SDL_Window* window);
 bool ImGui_ImplSDL2_InitForSDLRenderer(SDL_Window* window,SDL_Renderer* renderer);
 bool ImGui_ImplSDL2_InitForOther(SDL_Window* window);
 void ImGui_ImplSDL2_Shutdown(void);
 void ImGui_ImplSDL2_NewFrame(void);
 bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
 void ImGui_ImplSDL2_SetGamepadMode(ImGui_ImplSDL2_GamepadMode mode,struct _SDL_GameController** manual_gamepads_array,int manual_gamepads_count);
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Gamepad SDL_Gamepad;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Gamepad;
typedef union SDL_Event SDL_Event;
typedef enum { ImGui_ImplSDL3_GamepadMode_AutoFirst, ImGui_ImplSDL3_GamepadMode_AutoAll, ImGui_ImplSDL3_GamepadMode_Manual }ImGui_ImplSDL3_GamepadMode; bool ImGui_ImplSDL3_InitForOpenGL(SDL_Window* window,void* sdl_gl_context);
 bool ImGui_ImplSDL3_InitForVulkan(SDL_Window* window);
 bool ImGui_ImplSDL3_InitForD3D(SDL_Window* window);
 bool ImGui_ImplSDL3_InitForMetal(SDL_Window* window);
 bool ImGui_ImplSDL3_InitForSDLRenderer(SDL_Window* window,SDL_Renderer* renderer);
 bool ImGui_ImplSDL3_InitForOther(SDL_Window* window);
 void ImGui_ImplSDL3_Shutdown(void);
 void ImGui_ImplSDL3_NewFrame(void);
 bool ImGui_ImplSDL3_ProcessEvent(const SDL_Event* event);
 void ImGui_ImplSDL3_SetGamepadMode(ImGui_ImplSDL3_GamepadMode mode,SDL_Gamepad** manual_gamepads_array,int manual_gamepads_count);

 // Helpers: UTF-8 <> wchar
int igImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end);      // return output UTF-8 bytes count 
int igImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);          // return input UTF-8 bytes count 
int igImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining);   // return input UTF-8 bytes count 
int igImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                            // return number of UTF-8 code-points (NOT bytes count) 
int igImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);                   // return number of bytes to express string as UTF-8 code-points 
]]