import sys
import os
from pywinauto import Application
import pywinauto

mouseClick= pywinauto.mouse

app = Application().connect(process = 18944)

mouseClick.click(coords=(292, 197))