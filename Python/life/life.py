from typing import List
from PyQt5.QtGui import QPixmap
from life.cell import Cell


class Life:

    def __init__(self, alive_pixmap: QPixmap, dead_pixmap: QPixmap) -> None:
        """
        :param alive_pixmap:
        :param dead_pixmap:
        """

        self._alive_pixmap: QPixmap = alive_pixmap
        self._dead_pixmap: QPixmap = dead_pixmap
        self._cells: List[List[Cell]] = []
    
    @property
    def cells(self) -> List[List[Cell]]:
        """
        :return:
        """

        return self._cells

    def open_initial_state(self, file_name: str) -> None:
        """
        :param file_name: file name with initial state.
        """

        with open(file_name, "r", encoding="utf-8") as file:
            content = file.read()
        self._cells.clear()
        self._cells = [[Cell(column, row, state, self._alive_pixmap, self._dead_pixmap)
                         for column, state in enumerate(line)]
                         for row, line in enumerate(content.split("\n"))]
