#include <cctype>
#include <memory.h>
#include <ctime>
#include <cstring>
#include <cstdio>
#include "conio21/conio2.h"
#include "appconsts.h"
#include "interface.h"

const int INFINITY = 10000000;
const int MAIN_DIAG_MARK = -1000000;

#define min(x, y) (x < y) ? x : y
#define DrawMainFrame drawFrame(78, 30, 2, 2)
#define PrintNullLine printf("                                                                          ")

void prepareMatrix(float** graph, int pointsCount) {
    for (int i = 0; i < pointsCount; i++) {
        for (int j = 0; j < pointsCount; j++) {
            if (i == j) graph[i][j] = MAIN_DIAG_MARK;
            else if (graph[i][j] == 0) graph[i][j] = INFINITY;
        }
    }
}

void floydAlgorithmOneStep(float** graph, int pointsCount, int iteration) {
    for (int i = 0; i < pointsCount; i++) {
        for (int j = 0; j < pointsCount; j++) {
            if (graph[i][iteration] != INFINITY && graph[iteration][j] != INFINITY && i != iteration && j != iteration) {
                if (graph[i][j] == INFINITY) {
                    graph[i][j] = graph[i][iteration] + graph[iteration][j];
                }
                else graph[i][j] = min(graph[i][j], graph[i][iteration] + graph[iteration][j]);
            }
        }
    }
}

void floydAlgorithm(float** graph, int pointsCount) {
    prepareMatrix(graph, pointsCount);
    for (int k = 0; k < pointsCount; k++) {
        for (int i = 0; i < pointsCount; i++) {
            for (int j = 0; j < pointsCount; j++) {
                if (graph[i][k] != INFINITY && graph[k][j] != INFINITY && i != k && j != k) {
                    if (graph[i][j] == INFINITY) {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                    else graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
                }
            }
        }
    }
}

void printMatrix(float** graph, int pointsCount, int ltcX, int ltcY) {
    for (int i = 0; i < pointsCount; i++) {
        for (int j = 0; j < pointsCount; j++) {
            gotoxy(ltcX + j * 7, ltcY + i);
            if (graph[i][j] == INFINITY) {
                printf("[ INF ]");
            }
            else if (graph[i][j] == MAIN_DIAG_MARK) {
                printf("[  X  ]");
            }
            else {
                printf("[%5.2f]", graph[i][j]);
            }
        }
    }
}

void writeMatrixToFile(float** graph, int pointsCount, FILE* file, bool afterFloyd = false) {
    fflush(file);
    fprintf(file, (afterFloyd) ? "�������, ����� ���������� ��������� ������\n" : "������� ������� ��������� �����:\n");
    for (int i = 0; i < pointsCount; i++) {
        for (int j = 0; j < pointsCount; j++) {
            if (graph[i][j] == INFINITY) {
                fprintf(file, "[   INF   ]");
            }
            else if (graph[i][j] == MAIN_DIAG_MARK) {
                fprintf(file, "[    X    ]");
            }
            else {
                fprintf(file, "[%9.3f]", graph[i][j]);
            }
        }
        fprintf(file, "\n");

    }
    fprintf(file, "\n");
}

int main() {
    char title[255];
    sprintf(title, "FuFloyd v. %s", VERSION);
    title[strlen(title)] = 0;
    setWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
    hideCursor();
    drawFrame(78, 30, 2, 2);
    setCP(1251);
    float** weightMatrix;
    int choice = 1;
    do {
        DrawMainFrame;
        drawFrame(20, 3, 30, 6);
        gotoxy(32, 7); printf(title);
        choice = drawMenu(choice, mainMenu, 4, 13, 12, 4, 2, 1);
        switch (choice) {
            case 1: {
                DrawMainFrame;
                int pointsCount = 0;
                do {
                    gotoxy(4, 4); printf("������� ���������� ������ � ����� (����. 8): ");
                    char rawPoints[4];
                    fgets(rawPoints, 4, stdin);
                    pointsCount = strtol(rawPoints, nullptr, 10);
                    fflush(stdin);
                    //scanf("%d", &pointsCount);
                    if (pointsCount < 1 || pointsCount > 8) {
                        gotoxy(4, 5); printf("������� ������� ���������� ������ �����.\n");
                        gotoxy(4, 6); printf("����������, ���������� ��� ���.\n");
                        gotoxy(4, 7); printf("������� Enter ��� �����������");
                        waitForKey(13);
                        gotoxy(4, 5); PrintNullLine;
                        gotoxy(4, 6); PrintNullLine;
                        gotoxy(4, 7); PrintNullLine;
                        gotoxy(4, 4); PrintNullLine;
                    }
                } while (pointsCount < 1 || pointsCount > 8);
                weightMatrix = new float* [pointsCount];
                for (int i = 0; i < pointsCount; i++) {
                    weightMatrix[i] = new float [pointsCount];
                }
                gotoxy(4, 5); printf("��������� � ���������� ������� ������� �����");
                for (int i = 0; i < pointsCount; i++) {
                    for (int j = 0; j < pointsCount; j++) {
                        gotoxy(4, 6 + j);
                        printf("������� ������� V[%d][%d]: ", i + 1, j + 1);
                        //gotoxy(29, 6 + j);
                        if (i == j) {
                            printf("[������� ������� ���������]");
                            weightMatrix[i][j] = MAIN_DIAG_MARK;
                        }
                        else {
                            do {
                                char rawWeight[16];
                                fgets(rawWeight, 16, stdin);
                                fflush(stdin);
                                if (rawWeight[0] == '0') {
                                    weightMatrix[i][j] = 0;
                                    break;
                                }
                                else {
                                    weightMatrix[i][j] = strtod(rawWeight, nullptr);
                                    if (weightMatrix[i][j] == 0) {
                                        gotoxy(4, 7 + j); printf("������� ����� ��� ����� �����.\n");
                                        gotoxy(4, 8 + j); printf("����������, ���������� ��� ���.\n");
                                        gotoxy(4, 9 + j); printf("������� Enter ��� �����������");
                                        waitForKey(13);
                                        gotoxy(4, 7 + j); PrintNullLine;
                                        gotoxy(4, 8 + j); PrintNullLine;
                                        gotoxy(4, 9 + j); PrintNullLine;
                                        gotoxy(4, 6 + j); PrintNullLine;
                                        gotoxy(4, 6 + j);
                                        printf("������� ������� V[%d][%d]: ", i + 1, j + 1);
                                    }
                                    else break;
                                }
                            } while (true);
                            //scanf("%f", &weightMatrix[i][j]);
                        }
                    }
                    for (int j = 0; j < pointsCount; j++) {
                        gotoxy(4, 6 + j); PrintNullLine;
                    }
                }
                gotoxy(4, 5); printf("������� ������� ����� ������� ���������.    ");// getchar();
                int outputMode;
                gotoxy(4, 6); printf("���� ������� �������� �������?");
                do {
                    gotoxy(4, 7); printf("1 - � ���� (���������� �������� ������), 2 - � �������: ");
                    char rawOutputMode[3]; fgets(rawOutputMode, 3, stdin);
                    outputMode = strtol(rawOutputMode, nullptr, 10);
                    fflush(stdin);
                    //scanf("%d", &outputMode);
                    if (outputMode != 1 && outputMode != 2) {
                        gotoxy(4, 8); printf("������� ����� ����� ������\n");
                        gotoxy(4, 9); printf("����������, ���������� ��� ���.\n");
                        gotoxy(4, 10); printf("������� Enter ��� �����������");
                        waitForKey(13);
                        gotoxy(4, 8); PrintNullLine;
                        gotoxy(4, 9); PrintNullLine;
                        gotoxy(4, 10); PrintNullLine;
                        gotoxy(4, 7); PrintNullLine;
                    }
                } while (outputMode != 1 && outputMode != 2);
                gotoxy(4, 6); PrintNullLine;
                gotoxy(4, 7); PrintNullLine;
                switch (outputMode) {
                    case 1: {
                        time_t sc = time(nullptr);
                        char outputFile[255]; sprintf(outputFile, "graph-%lld.txt", (long long) sc);
                        outputFile[strlen(outputFile)] = 0;
                        FILE* out = fopen(outputFile, "w+");
                        writeMatrixToFile(weightMatrix, pointsCount, out);
                        floydAlgorithm(weightMatrix, pointsCount);
                        writeMatrixToFile(weightMatrix, pointsCount, out, true);
                        gotoxy(4, 6); printf("������� ������ �������� � ���� %s", outputFile);

                        //gotoxy(4, 7); printf("1 - � ���� (���������� �������� ������), 2 - � �������: ");
                        int wish;
                        do {
                            gotoxy(4, 7); printf("�� ������ ������ ����� ������-������ ����������� ����? (1 - ��; 0 - ���)");
                            char rawWish[3];
                            fgets(rawWish, 3, stdin);
                            fflush(stdin);
                            if (rawWish[0] == '0') wish = 2;
                            else wish = strtol(rawWish, nullptr, 10);
                            //scanf("%d", &stepByStep);
                            if (wish != 1 && wish != 2) {
                                gotoxy(4, 8); printf("������� ����� ��������\n");
                                gotoxy(4, 9); printf("����������, ���������� ��� ���.\n");
                                gotoxy(4, 10); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                gotoxy(4, 8); PrintNullLine;
                                gotoxy(4, 9); PrintNullLine;
                                gotoxy(4, 10); PrintNullLine;
                                gotoxy(4, 7); PrintNullLine;
                            }
                            //gotoxy(4, 6); PrintNullLine;
                        } while(wish != 1 && wish != 2);
                        gotoxy(4, 7); PrintNullLine;
                        switch (wish) {
                            case 1: {
                                int source, destination;
                                do {
                                    gotoxy(4, 8); printf("������� ����� ������� ������ ����:");
                                    char rawSource[3];
                                    fgets(rawSource, 3, stdin);
                                    fflush(stdin);
                                    source = strtol(rawSource, nullptr, 10);
                                    //scanf("%d", &stepByStep);
                                    if (source == 0 || source > pointsCount) {
                                        gotoxy(4, 9); printf("������� ����� ����� ������� ������ ����\n");
                                        gotoxy(4, 10); printf("����������, ���������� ��� ���.\n");
                                        gotoxy(4, 11); printf("������� Enter ��� �����������");
                                        waitForKey(13);
                                        gotoxy(4, 9); PrintNullLine;
                                        gotoxy(4, 10); PrintNullLine;
                                        gotoxy(4, 11); PrintNullLine;
                                        gotoxy(4, 8); PrintNullLine;
                                    }
                                    //gotoxy(4, 6); PrintNullLine;
                                } while(source == 0 || source > pointsCount);
                                do {
                                    gotoxy(4, 9); printf("������� ����� ������� ����� ����:");
                                    char rawDest[3];
                                    fgets(rawDest, 3, stdin);
                                    fflush(stdin);
                                    destination = strtol(rawDest, nullptr, 10);
                                    //scanf("%d", &stepByStep);
                                    if (destination == 0 || destination > pointsCount) {
                                        gotoxy(4, 10); printf("������� ����� ����� ������� ����� ����\n");
                                        gotoxy(4, 11); printf("����������, ���������� ��� ���.\n");
                                        gotoxy(4, 12); printf("������� Enter ��� �����������");
                                        waitForKey(13);
                                        gotoxy(4, 10); PrintNullLine;
                                        gotoxy(4, 11); PrintNullLine;
                                        gotoxy(4, 12); PrintNullLine;
                                        gotoxy(4, 9); PrintNullLine;
                                    }
                                    //gotoxy(4, 6); PrintNullLine;
                                } while(destination == 0 || destination > pointsCount);
                                float pathLen = weightMatrix[source-1][destination-1];
                                gotoxy(4, 10);
                                if (pathLen == MAIN_DIAG_MARK) {
                                    printf("�� ������� ������� ������� ��������� �������");
                                }
                                else if (pathLen == INFINITY) {
                                    printf("���� �� ������� %d � ������� %d �� ����������", source, destination);
                                }
                                else {
                                    printf("����� ����������� ���� �� ������� %d � ������� %d ����� %.2f", source, destination, pathLen);
                                }

                                gotoxy(4, 11); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                break;
                            }
                        }
                        break;
                    }
                    case 2: {
                        int stepByStep;
                        do {
                            gotoxy(4, 6); printf("��������� �������� ��������? (1 - ��, 0 - ���) ");
                            char rawStepByStep[3];
                            fgets(rawStepByStep, 3, stdin);
                            fflush(stdin);
                            if (rawStepByStep[0] == '0') stepByStep = 2;
                            else stepByStep = strtol(rawStepByStep, nullptr, 10);
                            //scanf("%d", &stepByStep);
                            if (stepByStep != 1 && stepByStep != 2) {
                                gotoxy(4, 7); printf("������� ����� ����� ����������� ����������\n");
                                gotoxy(4, 8); printf("����������, ���������� ��� ���.\n");
                                gotoxy(4, 9); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                gotoxy(4, 7); PrintNullLine;
                                gotoxy(4, 8); PrintNullLine;
                                gotoxy(4, 9); PrintNullLine;
                                gotoxy(4, 6); PrintNullLine;
                            }
                            gotoxy(4, 6); PrintNullLine;
                        } while(stepByStep != 1 && stepByStep != 2);
                        switch (stepByStep) {
                            case 1: {
                                prepareMatrix(weightMatrix, pointsCount);
                                gotoxy(4, 6); printf("���������� ��������� ������ ��������");
                                for (int k = 0; k < pointsCount; k++) {
                                    floydAlgorithmOneStep(weightMatrix, pointsCount, k);
                                    gotoxy(4, 7); printf("�������� %d/%d", k + 1, pointsCount);
                                    printMatrix(weightMatrix, pointsCount, 4, 9);
                                    gotoxy(4, 10 + pointsCount); printf("Enter = ��������� ��������");
                                    waitForKey(13);
                                }
                                gotoxy(4, 7); printf("�������, ����� ���������� ��������� ������:");
                                gotoxy(4, 10 + pointsCount); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                break;
                            }
                            case 2: {
                                floydAlgorithm(weightMatrix, pointsCount);
                                gotoxy(4, 7); printf("�������, ����� ���������� ��������� ������:");
                                printMatrix(weightMatrix, pointsCount, 4, 9);
                                gotoxy(4, 10 + pointsCount); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                break;
                            }
                        }
                        int wish;
                        do {
                            gotoxy(4, 10 + pointsCount); printf("�� ������ ������ ����� ������-������ ����������� ����? (1 - ��; 0 - ���)");
                            char rawWish[3];
                            fgets(rawWish, 3, stdin);
                            fflush(stdin);
                            if (rawWish[0] == '0') wish = 2;
                            else wish = strtol(rawWish, nullptr, 10);
                            //scanf("%d", &stepByStep);
                            if (wish != 1 && wish != 2) {
                                gotoxy(4, 11 + pointsCount); printf("������� ����� ��������\n");
                                gotoxy(4, 12 + pointsCount); printf("����������, ���������� ��� ���.\n");
                                gotoxy(4, 13 + pointsCount); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                gotoxy(4, 11 + pointsCount); PrintNullLine;
                                gotoxy(4, 12 + pointsCount); PrintNullLine;
                                gotoxy(4, 13 + pointsCount); PrintNullLine;
                                gotoxy(4, 10+ pointsCount); PrintNullLine;
                            }
                            //gotoxy(4, 6); PrintNullLine;
                        } while(wish != 1 && wish != 2);
                        gotoxy(4, 10 + pointsCount); PrintNullLine;
                        switch (wish) {
                            case 1: {
                                for (int i = 6; i < 20; i++) {
                                    gotoxy(4, i); PrintNullLine;
                                }
                                int source, destination;
                                do {
                                    gotoxy(4, 6); printf("������� ����� ������� ������ ����:");
                                    char rawSource[3];
                                    fgets(rawSource, 3, stdin);
                                    fflush(stdin);
                                    source = strtol(rawSource, nullptr, 10);
                                    //scanf("%d", &stepByStep);
                                    if (source == 0 || source > pointsCount) {
                                        gotoxy(4, 7); printf("������� ����� ����� ������� ������ ����\n");
                                        gotoxy(4, 8); printf("����������, ���������� ��� ���.\n");
                                        gotoxy(4, 9); printf("������� Enter ��� �����������");
                                        waitForKey(13);
                                        gotoxy(4, 7); PrintNullLine;
                                        gotoxy(4, 8); PrintNullLine;
                                        gotoxy(4, 9); PrintNullLine;
                                        gotoxy(4, 6); PrintNullLine;
                                    }
                                    //gotoxy(4, 6); PrintNullLine;
                                } while(source == 0 || source > pointsCount);
                                do {
                                    gotoxy(4, 7); printf("������� ����� ������� ����� ����:");
                                    char rawDest[3];
                                    fgets(rawDest, 3, stdin);
                                    fflush(stdin);
                                    destination = strtol(rawDest, nullptr, 10);
                                    //scanf("%d", &stepByStep);
                                    if (destination == 0 || destination > pointsCount) {
                                        gotoxy(4, 8); printf("������� ����� ����� ������� ����� ����\n");
                                        gotoxy(4, 9); printf("����������, ���������� ��� ���.\n");
                                        gotoxy(4, 10); printf("������� Enter ��� �����������");
                                        waitForKey(13);
                                        gotoxy(4, 8); PrintNullLine;
                                        gotoxy(4, 9); PrintNullLine;
                                        gotoxy(4, 10); PrintNullLine;
                                        gotoxy(4, 7); PrintNullLine;
                                    }
                                    //gotoxy(4, 6); PrintNullLine;
                                } while(destination == 0 || destination > pointsCount);
                                float pathLen = weightMatrix[source-1][destination-1];
                                gotoxy(4, 8);
                                if (pathLen == MAIN_DIAG_MARK) {
                                    printf("�� ������� ������� ������� ��������� �������");
                                }
                                else if (pathLen == INFINITY) {
                                    printf("���� �� ������� %d � ������� %d �� ����������", source, destination);
                                }
                                else {
                                    printf("����� ����������� ���� �� ������� %d � ������� %d ����� %.2f", source, destination, pathLen);
                                }

                                gotoxy(4, 9); printf("������� Enter ��� �����������");
                                waitForKey(13);
                                break;
                            }
                        }
                        break;
                    }
                }
                for (int i = 0; i < pointsCount; i++) delete [] weightMatrix[i];
                delete [] weightMatrix;
                break;
            }
            case 2: {
                DrawMainFrame;
                int pointsCount = 0;
                char graphFileName[255];
                gotoxy(4, 4); printf("������� ��� ����� �����: ");
                fgets(graphFileName, 255, stdin);
                graphFileName[strlen(graphFileName) - 1] = 0;
                FILE* graphFile = fopen(graphFileName, "r");
                if (graphFile != nullptr) {
                    fscanf(graphFile, "%d", &pointsCount);
                    weightMatrix = new float* [pointsCount];
                    for (int i = 0; i < pointsCount; i++) {
                        weightMatrix[i] = new float [pointsCount];
                    }
                    char line[512]; line[0] = 0;
                    fgets(line, 512, graphFile);
                    for (int i = 0; i < pointsCount; i++) {
                        int index = 0;
                        fgets(line, 512, graphFile);
                        char * tokenPointer = strtok(line, " ");
                        while (tokenPointer != nullptr) {
                            weightMatrix[i][index++] = (float) atof(tokenPointer);
                            tokenPointer = strtok(nullptr, " ");
                        }
                    }

                    time_t sc = time(nullptr);
                    char outputFile[255]; sprintf(outputFile, "graph-%lld.txt", (long long) sc);
                    outputFile[strlen(outputFile)] = 0;
                    FILE* out = fopen(outputFile, "w+");

                    writeMatrixToFile(weightMatrix, pointsCount, out);
                    floydAlgorithm(weightMatrix, pointsCount);
                    writeMatrixToFile(weightMatrix, pointsCount, out, true);
                    fclose(out);
                    gotoxy(4, 5); printf("������� ������ �������� � ���� %s", outputFile);
                    gotoxy(4, 6); printf("������� Enter ��� �����������");
                    waitForKey(13);
                    int wish;
                    do {
                        gotoxy(4, 7); printf("�� ������ ������ ����� ������-������ ����������� ����? (1 - ��; 0 - ���)");
                        char rawWish[3];
                        fgets(rawWish, 3, stdin);
                        fflush(stdin);
                        if (rawWish[0] == '0') wish = 2;
                        else wish = strtol(rawWish, nullptr, 10);
                        //scanf("%d", &stepByStep);
                        if (wish != 1 && wish != 2) {
                            gotoxy(4, 8); printf("������� ����� ��������\n");
                            gotoxy(4, 9); printf("����������, ���������� ��� ���.\n");
                            gotoxy(4, 10); printf("������� Enter ��� �����������");
                            waitForKey(13);
                            gotoxy(4, 8); PrintNullLine;
                            gotoxy(4, 9); PrintNullLine;
                            gotoxy(4, 10); PrintNullLine;
                            gotoxy(4, 7); PrintNullLine;
                        }
                        //gotoxy(4, 6); PrintNullLine;
                    } while(wish != 1 && wish != 2);
                    gotoxy(4, 7); PrintNullLine;
                    switch (wish) {
                        case 1: {

                            int source, destination;
                            do {
                                gotoxy(4, 8); printf("������� ����� ������� ������ ����: ");
                                char rawSource[3];
                                fgets(rawSource, 3, stdin);
                                fflush(stdin);
                                source = strtol(rawSource, nullptr, 10);
                                //scanf("%d", &stepByStep);
                                if (source == 0 || source > pointsCount) {
                                    gotoxy(4, 9); printf("������� ����� ����� ������� ������ ����\n");
                                    gotoxy(4, 10); printf("����������, ���������� ��� ���.\n");
                                    gotoxy(4, 11); printf("������� Enter ��� �����������");
                                    waitForKey(13);
                                    gotoxy(4, 9); PrintNullLine;
                                    gotoxy(4, 10); PrintNullLine;
                                    gotoxy(4, 11); PrintNullLine;
                                    gotoxy(4, 8); PrintNullLine;
                                }
                                //gotoxy(4, 6); PrintNullLine;
                            } while(source == 0 || source > pointsCount);
                            do {
                                gotoxy(4, 9); printf("������� ����� ������� ����� ����: ");
                                char rawDest[3];
                                fgets(rawDest, 3, stdin);
                                fflush(stdin);
                                destination = strtol(rawDest, nullptr, 10);
                                //scanf("%d", &stepByStep);
                                if (destination == 0 || destination > pointsCount) {
                                    gotoxy(4, 10); printf("������� ����� ����� ������� ����� ����\n");
                                    gotoxy(4, 11); printf("����������, ���������� ��� ���.\n");
                                    gotoxy(4, 12); printf("������� Enter ��� �����������");
                                    waitForKey(13);
                                    gotoxy(4, 10); PrintNullLine;
                                    gotoxy(4, 11); PrintNullLine;
                                    gotoxy(4, 12); PrintNullLine;
                                    gotoxy(4, 9); PrintNullLine;
                                }
                                //gotoxy(4, 6); PrintNullLine;
                            } while(destination == 0 || destination > pointsCount);
                            float pathLen = weightMatrix[source-1][destination-1];
                            gotoxy(4, 10);
                            if (pathLen == MAIN_DIAG_MARK) {
                                printf("�� ������� ������� ������� ��������� �������");
                            }
                            else if (pathLen == INFINITY) {
                                printf("���� �� ������� %d � ������� %d �� ����������", source, destination);
                            }
                            else {
                                printf("����� ����������� ���� �� ������� %d � ������� %d ����� %.2f", source, destination, pathLen);
                            }

                            gotoxy(4, 11); printf("������� Enter ��� �����������");
                            waitForKey(13);
                            break;
                        }
                    }

                    //fclose(out);
                    for (int i = 0; i < pointsCount; i++) delete [] weightMatrix[i];
                    delete [] weightMatrix;
                }
                else {
                    gotoxy(4, 5); printf("������ ����� �� ����������");
                    gotoxy(4, 6); printf("������� Enter ��� �����������");
                    waitForKey(13);
                }
                break;
            }
            case 3: {
                DrawMainFrame;
                gotoxy(4, 4); printf(title);
                gotoxy(4, 6); printf("�����: ���� \"TheSwagVader\" ������");
                gotoxy(4, 7); printf("����������� �������: github.com/TheSwagVader/FuFloyd");

                gotoxy(4, 9); printf("����� ���������� ����� � ����� � ������� ��������� ������");
                gotoxy(4, 10); printf("����� ������������ �������, �������� � ���������, � �� ������");

                gotoxy(4, 12); printf("������ ������������ �� ������ Apache 2.0. License");
                gotoxy(4, 15); printf("������� Enter ��� �����������");
                waitForKey(13);

                break;
            }
            default: {
                return 0;
            }
        }
    } while (choice != 4);
}
