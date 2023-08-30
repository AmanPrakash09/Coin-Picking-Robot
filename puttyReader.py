import pyautogui
import time

while True:
    time.sleep(0.25)
    pyautogui.moveTo(100, 75, 0.25)
    pyautogui.click()
    time.sleep(1)
    pyautogui.dragTo(0, 75, 0.25)
    pyautogui.hotkey('ctrl', 'c')
    time.sleep(0.25)

    # #pyautogui.press("backspace")
    # pyautogui.keyDown('alt')
    # pyautogui.---------------------------
    # PuTTY Fatal Error
    # ---------------------------
    # Error reading from serial device
    # ---------------------------
    # OK
    # ---------------------------press('tab')
    # pyautogui.press('tab')
    # pyautogui .keyUp('alt')

    pyautogui.moveTo(1100, 2050, 0.25)
    pyautogui.click()
    pyautogui.hotkey('ctrl', 'v')
    pyautogui.press('enter')
