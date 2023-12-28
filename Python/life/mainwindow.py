import os
from PyQt5.uic import loadUi
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5.QtWidgets import QFileDialog, QMainWindow
from life.life import Life
from life.lifewidget import LifeWidget
from life.version import VERSION


class MainWindow(QMainWindow):

    def __init__(self) -> None:
        super().__init__()
        self._dir_base: str = os.path.abspath(os.path.curdir)
        self._dir_media: str = os.path.join(self._dir_base, "media")
        self._dir_pattern: str = "."
        self._alive_pixmap: QPixmap = QPixmap(os.path.join(self._dir_media, "alive.png"))
        self._dead_pixmap: QPixmap = QPixmap(os.path.join(self._dir_media, "dead.png"))
        self._life: Life = Life(self._alive_pixmap, self._dead_pixmap)
        self._init_ui()

    def _init_ui(self) -> None:
        loadUi(os.path.join(self._dir_media, "mainwindow.ui"), self)
        self.setWindowTitle(f"GameOfLife v{VERSION}")
        self.setWindowIcon(QIcon(os.path.join(self._dir_media, "icon.png")))
        self.button_generate_random_init_state.clicked.connect(self.generate_random_initial_state)
        self.button_open_init_state.clicked.connect(self.open_initial_state)
        self.button_start_stop.clicked.connect(self.start_or_stop)
        self.life_widget: LifeWidget = LifeWidget()
        self.vertical_layout.addWidget(self.life_widget, 1)

    @pyqtSlot()
    def generate_random_initial_state(self) -> None:
        pass

    @pyqtSlot()
    def open_initial_state(self) -> None:
        file_name = QFileDialog.getOpenFileName(self, "Open file", self._dir_pattern, "Files (*.txt)")[0]
        if file_name:
            if not file_name.lower().endswith(".txt"):
                file_name = f"{file_name}.txt"
            if os.path.exists(file_name):
                self._dir_pattern = os.path.dirname(file_name)
                self._life.open_initial_state(file_name)
                self.life_widget.update_scene(self._life.cells)

    @pyqtSlot()
    def start_or_stop(self) -> None:
        pass
