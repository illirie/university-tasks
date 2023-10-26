from typing import Union, List, Dict
from datetime import date
import json
import csv


class LabWorkSession:
    def __init__(self, presence: bool, lab_work_number: int, lab_work_mark: int, lab_work_date: date):
        self.presence = presence
        self.lab_work_n = None if lab_work_number < 0 else lab_work_number
        self.lab_work_mark = -1 if lab_work_mark < 0 or lab_work_mark > 5 else lab_work_mark
        self.lab_work_date = lab_work_date

    def __str__(self) -> str:
        day = self.lab_work_date.day
        month = self.lab_work_date.month
        year = self.lab_work_date.year
        return f'{{"presence": {self.presence}, "lab_work_n": {self.lab_work_n}, "lab_work_mark": {self.lab_work_mark}, "date": "{year}:{month}:{day}"}}'

class Student:

    def __init__(self, unique_id: int, name: str, surname: str, group: int, subgroup: int) -> None:
        self.unique_id = unique_id
        self.name = name
        self.surname = surname
        self.group = None if group < 0 else group
        self.subgroup = None if subgroup < 0 else subgroup
        self.lab_work_sessions = []

    def __str__(self) -> str:
        stud = f'"unique_id": {self.unique_id}, "name": "{self.name}", "surname": "{self.surname}", "group": {self.group}, "subgroup": {self.subgroup}, "lab_works_sessions": [ '
        for i in self.lab_work_sessions:
            stud = stud + str(i) + ','
        return '{' + stud[:-1] + ']}'

    def append_lab_work_session(self, session: LabWorkSession):
        self.lab_work_sessions.append(session)

    def str_for_cvs(self, lab_n: int) -> str:
        day = self.lab_work_sessions[lab_n].lab_work_date.day
        month = self.lab_work_sessions[lab_n].lab_work_date.month
        year = self.lab_work_sessions[lab_n].lab_work_date.year
        stud = f'{self.unique_id};"{self.surname}";"{self.name}";{self.group};{self.subgroup};"{year}:{month}:{day}";{int(self.lab_work_sessions[lab_n].presence)};{self.lab_work_sessions[lab_n].lab_work_n};{self.lab_work_sessions[lab_n].lab_work_mark}'
        return stud

def load_students_json(file_path: str) -> Union[List[Student], None]:
    with open(file_path, 'rt', encoding='utf-8') as f:
        templates = json.load(f)
    students = []
    for e in templates['students']:
        a = Student(e['unique_id'], e['name'], e['surname'], e['group'], e['subgroup'])
        for i in e["lab_works_sessions"]:
            session = LabWorkSession(i['presence'], i['lab_work_n'], i['lab_work_mark'], date(*tuple(map(int, i['date'].split(':')))))
            a.append_lab_work_session(session)
        students.append(a)
    f.close()
    return students

def save_students_json(file_path: str, students: List[Student]) -> None:
    f = open(file_path, 'wt', encoding='utf-8')
    print('{"students":[', file=f)
    for i in students[:len(students) - 1]:
       print(str(i) + ',', file=f)
    print(str(students[len(students)-1]) + ']}', file=f)
    f.close()

def load_students_csv(file_path: str) -> Union[List[Student], None]:
    f = open(file_path, 'rt', encoding='utf-8')
    reader = list(csv.reader(f, delimiter=';'))
    students = []
    for i in range(1, len(reader)):
        a = Student(int(reader[i][0]), reader[i][2], reader[i][1], int(reader[i][3]), int(reader[i][4]))
        session = LabWorkSession(bool(reader[i][6]), int(reader[i][7]), int(reader[i][8]), date(*tuple(map(int, reader[i][5].split(':')))))
        if (a.unique_id in set([i.unique_id for i in students])):
            students[a.unique_id].append_lab_work_session(session)
        else:
            a.append_lab_work_session(session)
            students.append(a)
    f.close()
    return students

def save_students_csv(file_path: str, students: List[Student]) -> None:
    f = open(file_path, 'wt', encoding='utf-8')
    print("unique_id;name;surname;group;subgroup;date;presence;lab_work_number;lab_work_mark", file=f)
    for i in range(4):
        for j in students:
            print(j.str_for_cvs(i), file=f)
    f.close()

def main():
    a = load_students_json("students.json")
    save_students_json("students1.json", a)
    a = load_students_csv("students.csv")
    save_students_csv("students1.csv", a)

if __name__ == '__main__':
    main()