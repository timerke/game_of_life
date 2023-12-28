from typing import Optional, Union
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QGraphicsPixmapItem


class Cell:

    def __init__(self, x: int, y: int, init_state: Union[int, str], alive_pixmap: QPixmap, dead_pixmap: QPixmap) -> None:
        """
        :param x:
        :param y:
        :param init_state: initial state of cell;
        :param alive_pixmpa:
        :param dead_pixmap:
        """

        self._alive_pixmap: QPixmap = alive_pixmap
        self._dead_pixmap: QPixmap = dead_pixmap
        self._pixmap_item: QGraphicsPixmapItem = QGraphicsPixmapItem(self._dead_pixmap)
        self._state: int = 0
        self._x: int = x
        self._y: int = y
        self.state = init_state
    
    @property
    def graphics_item(self) -> QGraphicsPixmapItem:
        """
        :return:
        """

        return self._pixmap_item

    @property
    def state(self) -> int:
        """
        return:
        """

        return self._state
    
    @state.setter
    def state(self, new_state: Union[int, str]) -> None:
        """
        :param new_state:
        """

        new_state = int(new_state)
        self._state = 1 if new_state != 0 else 0
        self._set_pixmap_item()
    
    def _set_pixmap_item(self) -> None:
        pixmap = self._alive_pixmap if self._state else self._dead_pixmap
        self._pixmap_item.setPixmap(pixmap)
