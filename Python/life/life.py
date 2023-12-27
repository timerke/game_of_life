from typing import List, Union
from life.lifewidget import LifeWidget


class Cell:

    def __init__(self, init_state: Union[int, str]) -> None:
        """
        :param init_state: initial state of cell.
        """

        self._state: int = int(init_state)


class Life:

    def __init__(self) -> None:
        self._cells: List[List[Cell]] = []
        self.life_widget: LifeWidget = LifeWidget()

    def open_initial_state(self, file_name: str) -> None:
        """
        :param file_name: file name with initial state.
        """

        with open(file_name, "r", encoding="utf-8") as file:
            content = file.read()
        self._cells.clear()
        self._cells = [[Cell(cell) for cell in line] for line in content.split("\n")]
