from typing import List
from PyQt5.QtWidgets import QGraphicsScene, QGraphicsView
from life.cell import Cell


class LifeWidget(QGraphicsView):

    def __init__(self) -> None:
        super().__init__()
        self._cells: List[List[Cell]] = []
        self._init_ui()
    
    def _init_ui(self) -> None:
        self._scene: QGraphicsScene = QGraphicsScene()
        self.setScene(self._scene)
    
    def update_scene(self, cells: List[List[Cell]]) -> None:
        """
        :param cells:
        """
        
        self._scene.clear()
        self._cells = cells
        for line in self._cells:
            for cell in line:
                self._scene.addItem(cell.graphics_item)
