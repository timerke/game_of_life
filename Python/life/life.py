from typing import List
from life.lifewidget import LifeWidget


class Cell:

    def __init__(self) -> None:
        pass


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
            for line in content.split():
                for cell in line:
                    pass
