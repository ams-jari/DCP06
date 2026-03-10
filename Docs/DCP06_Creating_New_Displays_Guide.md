# DCP06 Creating New Displays – Development Guide

This document describes how to create new displays (dialogs/screens) in DCP06, add text labels, configure function keys, and integrate them into the application. It is intended for developers who need to add multiple new displays accurately and consistently.

---

## 1. Overview: Display Architecture

DCP06 uses a **Controller + Dialog + Model** pattern:

- **Controller** – Handles user input (F-keys, navigation), owns the dialog, routes to sub-controllers.
- **Dialog** – The visible screen; holds UI controls (labels, inputs, layouts).
- **Model** – Optional data layer; dialogs derive from `GUI::ModelHandlerC` to get/set model.

Navigation flow: `SetActiveController(id, true)` or `SetActiveDialog(id, true)` switches the active screen.

**Reference implementations:** `Meas3DDialog` / `Meas3DController` (3dmeas), `FileController` / `FileDialog` (File.cpp), `SpecialMenuDialog` / `SpecialMenuController` (SpecialMenu.cpp).

---

## 2. Creating a New Display

### 2.1 Choose the Dialog Base Class

| Base Class | Use Case | Example |
|-----------|----------|---------|
| `GUI::BaseDialogC` | Form with controls (labels, inputs) | Meas3DDialog, XYZDialog, DefineLine |
| `GUI::GraphMenuDialogC` | Icon/graphic menu with items | MainMenu, SpecialMenu, OrientationMenu |
| `GUI::TableDialogC` | List/table view | SelectFile, HomePoints, ResCircle |
| `GUI::StandardDialogC` | Standard dialog layout | InfoDialog, LicenseDialog |

### 2.2 Add Dialog and Controller IDs

In `include/dcp06/core/Defs.hpp`:

```cpp
// Add a unique DIALOG ID (use next free number, ~170+)
#define MY_NEW_DLG  170

// Add a unique CONTROLLER ID (use next free number, ~75+)
#define MY_NEW_CONTROLLER  75
```

**Important:** IDs must be unique. Check existing ranges in Defs.hpp before adding new ones.

### 2.3 Define the Dialog Class

**Header (e.g. `include/dcp06/feature/MyNewDisplay.hpp`):**

```cpp
#include <dcp06/core/Types.hpp>
#include <GUI_StandardDialog.hpp>   // or BaseDialog, TableDialog, GraphMenuDialog
#include <GUI_ComboLineCtrl.hpp>    // for label+input controls

namespace DCP
{
    class Model;

    class MyNewDialog : public GUI::BaseDialogC, public GUI::ModelHandlerC
    {
    public:
        enum eCtrlId { eLabel1 = 1, eLabel2 };

        MyNewDialog(Model* pModel);

        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void RefreshControls();
        virtual bool SetModel(GUI::ModelC* pModel);
        Model* GetModel() const;

    protected:
        GUI::ComboLineCtrlC* m_pLabel1;
        GUI::ComboLineCtrlC* m_pLabel2;

    private:
        Model* m_pModel;
    };

    class MyNewController : public GUI::ControllerC
    {
    public:
        MyNewController(Model* pModel);
        virtual ~MyNewController();

        virtual void OnF1Pressed();
        virtual void OnF2Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);

    private:
        MyNewDialog* m_pDlg;
        Model* m_pModel;
    };
}
```

### 2.4 Implement the Controller Constructor

Register the dialog and function keys:

```cpp
#include <dcp06/core/Defs.hpp>

DCP::MyNewController::MyNewController(Model* pModel)
    : m_pDlg(nullptr), m_pModel(pModel)
{
    SetTitle(StringC(AT_DCP06, T_DCP_SOME_TITLE_TOK));  // see §3 for tokens

    m_pDlg = new MyNewDialog(pModel);
    (void)AddDialog(MY_NEW_DLG, m_pDlg, true);

    // Function keys (see §4)
    FKDef vDef;
    vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    SetFunctionKey(FK1, vDef);

    vDef.strLable = StringC(AT_DCP06, K_DCP_SOME_ACTION_TOK);
    SetFunctionKey(FK2, vDef);

    // Hide quit on SHFK6 (optional)
    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);
}
```

### 2.5 Wire Into the Application

In `src/application/Application.cpp` (or the controller that should open your display), add navigation:

```cpp
// When user selects your menu item or flow:
if (GetController(MY_NEW_CONTROLLER) == nullptr)
    (void)AddController(MY_NEW_CONTROLLER, new DCP::MyNewController(dynamic_cast<DCP::Model*>(GetModel())));
(void)GetController(MY_NEW_CONTROLLER)->SetModel(GetModel());
SetActiveController(MY_NEW_CONTROLLER, true);
```

To add a menu entry (e.g. in Main Menu), edit the appropriate `MenuDialog::OnInitDialog()` and use `AddItem()` with an ID that routes to your controller in `OnActiveDialogClosed` / `OnActiveControllerClosed`.

---

## 3. Creating Text Labels

### 3.1 Control Types for Labels

- **`GUI::ComboLineCtrlC`** – Label + value (most common):
  - `IC_String` – text input
  - `IC_Float` – numeric
  - Use for both labels and editable fields.

- **`GUI::TextCtrlC`** – Plain text, no value (see InfoDialog, AutoMatch).

### 3.2 Basic Label-Value Control

```cpp
// In OnInitDialog():
m_pLabel1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
m_pLabel1->SetId(eLabel1);
m_pLabel1->SetText(StringC(AT_DCP06, P_DCP_MY_LABEL_TOK));  // Left-side label text

// Read-only (display only):
m_pLabel1->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
m_pLabel1->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);

// Editable with max length:
m_pLabel1->GetStringInputCtrl()->SetCharsCountMax(64);
m_pLabel1->SetEmptyAllowed(true);

AddCtrl(m_pLabel1);  // or pLayout->AddCtrl(m_pLabel1);
```

### 3.3 Token System for Text

All user-visible strings should use **tokens** from the text database.

**Usage:**
```cpp
StringC(AT_DCP06, TOKEN_NAME)
```

**Token constants** are in `Text/DCP06_TOK.HPP` (auto-generated). To add new text:

1. Edit `Text/Languages/en/DCP06.men` – add a `<Token>` block with unique `Number`, `Name`, and `Text`. The constant in code is `Name` + `_TOK` (e.g. `P_DCP_MY_LABEL` → `P_DCP_MY_LABEL_TOK`).
2. Regenerate tokens using the Leica TextTool (VBNet) or project-specific build step.
3. Use the new constant in code: `StringC(AT_DCP06, P_DCP_MY_LABEL_TOK)`.

**Naming conventions:**
- `P_` – Parameter/label (e.g. "Point ID (PID)")
- `T_` – Title (e.g. "3D-MEAS")
- `K_` – Function key label (e.g. "CONT", "NEXT")
- `L_` – List/menu item text
- `M_` – Message text

**Temporary / non-translatable:** Use `L"Your text"` (e.g. `vDef.strLable = L"Import"`). Prefer tokens for production.

### 3.4 Layout (Multi-column)

```cpp
GUI::LayoutC* pLayout = new GUI::LayoutC(GUI::LayoutC::LT_HorizontalBox, this);
GUI::LayoutC* pCol1 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
GUI::LayoutC* pCol2 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
pLayout->AddLayout(pCol1);
pLayout->AddLayout(pCol2);
SetLayout(pLayout);

pCol1->AddCtrl(m_pLabel1);
pCol2->AddCtrl(m_pValue1);
```

---

## 4. Adding Buttons to Function Keys

### 4.1 Available Function Keys

| Key | Constant | Shift variant |
|-----|----------|---------------|
| F1 | FK1 | SHFK1 |
| F2 | FK2 | SHFK2 |
| F3 | FK3 | SHFK3 |
| F4 | FK4 | SHFK4 |
| F5 | FK5 | SHFK5 |
| F6 | FK6 | SHFK6 |

### 4.2 Setting Function Key Labels

```cpp
void MyController::show_function_keys()  // or in constructor
{
    ResetFunctionKeys();  // Clear existing

    FKDef vDef;
    vDef.poOwner = this;  // Required: controller that receives OnF1Pressed etc.

    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    SetFunctionKey(FK1, vDef);

    vDef.strLable = StringC(AT_DCP06, K_DCP_NEXT_TOK);
    SetFunctionKey(FK2, vDef);

    // ... FK3, FK4, FK5, FK6, SHFK1..SHFK6 ...

    // Hide or blank quit key:
    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);

    GUI::DesktopC::Instance()->UpdateFunctionKeys();  // Required after changes
}
```

### 4.3 Handling Key Presses

Override in your Controller:

```cpp
void MyController::OnF1Pressed()
{
    // Handle F1 action
    Close(EC_KEY_CONT);  // or navigate elsewhere
}

void MyController::OnF2Pressed()
{
    SetActiveController(SOME_OTHER_CONTROLLER, true);
}
```

### 4.4 Enable / Disable / Hide Keys

```cpp
DisableFunctionKey(FK1);   // Grayed out
EnableFunctionKey(FK1);   // Re-enable
HideFunctionKey(FK3);      // Not shown
```

Call `GUI::DesktopC::Instance()->UpdateFunctionKeys()` after changes.

### 4.5 Dynamic Keys

Many displays call `show_function_keys()` when state changes (e.g. point menu vs measure mode in 3dmeas). Implement `show_function_keys()` and call it from `OnControllerActivated`, `RefreshControls`, or after user actions.

---

## 5. Checklist for a New Display

- [ ] **Defs.hpp:** Add `#define MY_NEW_DLG` and `#define MY_NEW_CONTROLLER`
- [ ] **Header:** Declare `MyNewDialog` and `MyNewController`
- [ ] **Dialog:** `OnInitDialog()` – create controls, layout, `AddCtrl` / `AddLayout`
- [ ] **Dialog:** `OnDialogActivated()` – initialization when display is shown
- [ ] **Dialog:** `RefreshControls()` – update UI from model/data
- [ ] **Dialog:** `SetModel()` – wire model, call `ModelHandlerC::SetModel`
- [ ] **Controller:** Constructor – `AddDialog()`, `SetFunctionKey()` for FK1–FK6
- [ ] **Controller:** `OnF1Pressed()` … `OnF6Pressed()` / `OnSHF*Pressed()` – implement actions
- [ ] **Controller:** `SetModel()` – pass model to dialog
- [ ] **Controller:** `OnActiveDialogClosed` / `OnActiveControllerClosed` – handle sub-controller results
- [ ] **Tokens:** Add text tokens if needed; use `StringC(AT_DCP06, TOKEN)`
- [ ] **Application.cpp:** Add controller, call `SetActiveController(MY_NEW_CONTROLLER, true)` at entry point
- [ ] **Project:** Add new .cpp/.hpp to build (vcxproj / CMake)
- [ ] **Include:** `#include <dcp06/feature/MyNewDisplay.hpp>` where used

---

## 6. Sub-Controllers and Modal Flows

For flows that need a sub-screen (e.g. "Select Point", "Enter Text"):

1. **Create sub-controller** if it does not exist (e.g. `InputTextController`, `SelectPointController`).
2. **Add controller on demand:**
   ```cpp
   if (GetController(INPUT_TEXT_CONTROLLER) == nullptr)
       (void)AddController(INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pModel));
   GetController(INPUT_TEXT_CONTROLLER)->SetModel(pModel);
   SetActiveController(INPUT_TEXT_CONTROLLER, true);
   ```
3. **Handle result in `OnActiveControllerClosed`:**
   ```cpp
   if (lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
   {
       // User confirmed; get result and continue
   }
   ```

---

## 7. Common Pitfalls

1. **Forgetting `UpdateFunctionKeys()`** – Function key changes do not appear until `GUI::DesktopC::Instance()->UpdateFunctionKeys()` is called.
2. **Wrong `poOwner`** – `FKDef.poOwner` must be the controller that implements `OnF1Pressed` etc.
3. **Duplicate IDs** – Reusing dialog/controller IDs causes incorrect navigation.
4. **Missing `RefreshControls()`** – Data may not show if `RefreshControls()` is not called after model changes or when the dialog is activated.
5. **Token not defined** – Using a non-existent token compiles but shows nothing or wrong text; ensure tokens exist in DCP06_TOK.HPP.
6. **Model not passed** – `SetModel()` must be called on the controller when it becomes active (e.g. from `Application::OnActiveDialogClosed`).

---

## 8. Key File References

| Purpose | File |
|---------|------|
| **Captivate layout quirks** | `Docs/Captivate_GUI_Design_Notes.md` |
| Dialog/Controller IDs | `include/dcp06/core/Defs.hpp` |
| Text tokens (generated) | `Text/DCP06_TOK.HPP` |
| Text token source | `Text/Languages/en/DCP06.men` |
| App routing | `src/application/Application.cpp` |
| Form dialog example | `src/measurement/3dmeas.cpp`, `include/dcp06/measurement/3dmeas.hpp` |
| Menu dialog example | `src/core/SpecialMenu.cpp` |
| Table dialog example | `src/file/SelectFile.cpp`, `src/measurement/HomePoints.cpp` |
| Function keys example | `src/file/File.cpp` (lines ~383–426), `src/measurement/3dmeas.cpp` (show_function_keys) |

---

*Document version: 1.0 | Based on DCP06 codebase analysis*
