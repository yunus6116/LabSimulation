// Yunus Kara 1306180061 25.02.2020

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm> 
using namespace std;

//Tüm entitylerimizin verilerini atamak üzere olusturdugum değişkenler public de gerekli default değerler atanarak yapıldı.
class Entity { 
public:
    string name;
    vector<int> currentLocation;
    bool isEntityInfected=false;
    string genomesOfEntity;
    bool immune=false;
    bool isCanEntityRecovered=false;
    bool isCanEntityDead=false;
    bool isCanEntityInfected = false;
    bool Invisible = false;
    bool dead = false;
    bool isInIncubationTime = false;
    bool recovered = false;
    int removeDeadCount = 0;
    int recoveredCount = 0;
    int deadCount = 0;
    int incubationCount = 0;
    bool cantMove = false;

};



int main()
{
    //Tüm olmasa da bazı gerekli tanımlamalar başta yapıldı.
    vector<string> linesForEntity;
    vector<string> linesForTurns;
    vector<vector<string>> entities;
    vector<vector<string>> turns;
    int lineCount = 0;
    string entityData;
    string turnData;

    string delimiter = " ";

    ifstream okumaDosyasi;
    okumaDosyasi.open("input.txt");
    int number_of_lines = 0;
    string line;

    //Tüm satır sayısını elde ettim
    while (getline(okumaDosyasi, line)) {
        number_of_lines++;
    }
    okumaDosyasi.close();

    // İlerde sıkça kullanacağım size ve turn countları tuttum.
    okumaDosyasi.open("input.txt");
    int size =0;
    int turn_count = 0;
    string veri;
    if (okumaDosyasi.is_open()) {
        okumaDosyasi >> veri;
        okumaDosyasi >> size;
        okumaDosyasi >> veri;
        okumaDosyasi >> turn_count;
    }
    okumaDosyasi.close();
    // entity sayısını elde ettim.
    int entity_count = number_of_lines - turn_count - 2;

    //burada entity'lerin olduğu satırı çekip ordaki tüm verileri linesForEntity vectorume pushladım.
    ifstream OkumaDosyasi("input.txt");

    while (getline(OkumaDosyasi, entityData)) {
        lineCount++;
        if (lineCount > 2 && lineCount < entity_count + 3) {
            linesForEntity.push_back(entityData);
        }
    }
    //tüm entity verilerini parselleyip ilgili yere saklamak adına 2D olan entities vectorumun size'ını dinamik olacak şekilde tanımladım. 
    entities.resize(entity_count);
    for (int i = 0; i < entity_count; i++) {
        entities[i].resize(5);
    }
    //Tanınmlamış olduğu 2D verktörün içine uygun entities özelliklerini parça parça ekledim. 
    for (int i = 0; i < entity_count; i++) {
        size_t pos = 0;
        int wordCount = 0;
        while ((pos = linesForEntity[i].find(delimiter)) != string::npos) {
            entities[i][wordCount] = linesForEntity[i].substr(0, pos);
            linesForEntity[i].erase(0, pos + delimiter.length());
            wordCount++;
        }
        entities[i][wordCount] = linesForEntity[i].substr(0, pos);
    }
    OkumaDosyasi.close();

    //Burada verilen entity sayısı kadar entity oluşturup her bir entity'e sahip oldukları temel değerler atandı.(name, genleri, konum vs)
    vector<Entity> allDataForEntities;
    allDataForEntities.resize(entity_count);
    for (int i = 0; i < entity_count; i++) {

        allDataForEntities[i].name = entities[i][1];
        allDataForEntities[i].genomesOfEntity = entities[i][2];
        allDataForEntities[i].currentLocation.resize(entities[i][3].size() - 1);

        allDataForEntities[i].currentLocation[0] = stoi(entities[i][3].substr(0, entities[i][3].find('x')));
        allDataForEntities[i].currentLocation[1] = stoi(entities[i][3].substr(entities[i][3].find('x') + 1, entities[i][3].size() - 1));
        if (entities[i][4] == "infected") {
            allDataForEntities[i].isInIncubationTime = true;
            //allDataForEntities[i].isEntityInfected = true;
        }
        else {
            //allDataForEntities[i].isEntityInfected= false;
            allDataForEntities[i].isInIncubationTime = false;
        }
    }
    // Entitylerin default verilen genomlarını kontrol ettim gerekli durumlarda gerkli atamaları yaptım.
    for (int i = 0; i < entity_count; i++) {
        int abCount = 0;
        int abcdCount = 0;
        for (int j = 0; j < 5; j++) {
            if (allDataForEntities[i].genomesOfEntity[j] == 'A' || allDataForEntities[i].genomesOfEntity[j] == 'B') {
                abCount++;
                abcdCount++;
            }
            else if (allDataForEntities[i].genomesOfEntity[j] == 'C' || allDataForEntities[i].genomesOfEntity[j] == 'D') {
                abcdCount++;
            }
        }
        if (abCount == 0 && abcdCount==0) {
            allDataForEntities[i].immune = true;

        }
        else if (abCount > 0 && abcdCount < 3) {
            allDataForEntities[i].isCanEntityRecovered = true;
            allDataForEntities[i].isCanEntityInfected = true;

        }
        else if (abCount > 0 && abcdCount >= 3) {
            allDataForEntities[i].isCanEntityDead = true;
            allDataForEntities[i].isCanEntityInfected = true;

        }
    }
    //Input'ta verilen turn verilerinin hepsini linesForTurns vectörüme pushladım.
    ifstream OkumaDosyasi2("input.txt");
    lineCount = 0;
    while (getline(OkumaDosyasi2, turnData)) {
        lineCount++;
        if (lineCount > entity_count + 2 && lineCount < number_of_lines + 1) {
            linesForTurns.push_back(turnData);
        }
    }
    //Tüm verileri parselleyip ayrı ayrı yerleştirmek için 2D oluşturmuş olduğum turns vectörümün size'ını dinamik hale getirdim.
    turns.resize(turn_count);
    for (int i = 0; i < turn_count; i++) {
        turns[i].resize(entity_count + 3);
    }

    //Parselleyip içine ayrı ayrı yerleştirmek için oluşturduğum 2D vectörümün içine turn değerleri tek tek yerleşti.
    for (int i = 0; i < linesForTurns.size(); i++) {
        size_t pos = 0;
        int wordCount = 0;
        while ((pos = linesForTurns[i].find(delimiter)) != string::npos) {
            turns[i][wordCount] = linesForTurns[i].substr(0, pos);
            linesForTurns[i].erase(0, pos + delimiter.length());
            wordCount++;
        }
        turns[i][wordCount] = linesForTurns[i].substr(0, pos);
    }
    //Gerekli countlar tanımlandı.
    int normalCount = 0;
    int infectedcount = 0;
    int recoveredCount = 0;
    int deadcount = 0;
    
    ofstream output2;
    output2.open("turns.txt");
    //Tüm turnleri dönüyoruz
    for (int i = 0; i < turn_count; i++) {

        //Entityler arasındaki minumum mesafeyi ölçtüm ve duruma göre kluçkaya soktum.
        for (int l = 0; l < entity_count; l++) {
            for (int m = 0; m < entity_count; m++) {
                if (allDataForEntities[m].Invisible == false && allDataForEntities[l].Invisible == false
                    && l != m
                    && (allDataForEntities[m].isEntityInfected == true && allDataForEntities[l].isEntityInfected == false || allDataForEntities[m].isEntityInfected == false && allDataForEntities[l].isEntityInfected == true)
                    && allDataForEntities[m].dead == false && allDataForEntities[l].dead == false
                    && allDataForEntities[m].immune == false && allDataForEntities[l].immune == false
                    && allDataForEntities[m].isInIncubationTime==false && allDataForEntities[l].isInIncubationTime==false) {
                    int diffarenceOfDistanceY = 0;
                    int diffarenceOfDistanceX = 0;
                    diffarenceOfDistanceY = abs(allDataForEntities[l].currentLocation[0] - allDataForEntities[m].currentLocation[0]);
                    diffarenceOfDistanceX = abs(allDataForEntities[l].currentLocation[1] - allDataForEntities[m].currentLocation[1]);
                    //Entitylerin infecte özellekliklerine göre bir entity infecte olmus bir entitye 3<= yakına girdiği anda kuluçkaya soktuk.
                    if (allDataForEntities[l].isEntityInfected && allDataForEntities[m].isCanEntityInfected && diffarenceOfDistanceX <= 3 && diffarenceOfDistanceY <= 3) {
                        allDataForEntities[m].isInIncubationTime = true;
                    }
                    else if (allDataForEntities[m].isEntityInfected && allDataForEntities[l].isCanEntityInfected && diffarenceOfDistanceX <= 3 && diffarenceOfDistanceY <= 3) {
                        allDataForEntities[l].isInIncubationTime = true;                      
                    }
                }
            }
        }
    
        for (int s = 0; s < entity_count; s++) {

            //Görünür olup olmamasına bakıyoruz. Ölmüşse görünür ve incelememiz başlar.
            if (allDataForEntities[s].Invisible == false) {
                //Gerekli zamanda counterlarımızı arttırıyoruz.
                if (allDataForEntities[s].isInIncubationTime) {
                    allDataForEntities[s].incubationCount++;
                }

                else if (allDataForEntities[s].isEntityInfected && allDataForEntities[s].isCanEntityRecovered && allDataForEntities[s].immune == false) {
                    allDataForEntities[s].recoveredCount++;
                }

                else if (allDataForEntities[s].dead == true && allDataForEntities[s].isEntityInfected == true) {
                    allDataForEntities[s].removeDeadCount++;
                }

                else if (allDataForEntities[s].isEntityInfected && allDataForEntities[s].isCanEntityDead && allDataForEntities[s].dead == false) {
                    allDataForEntities[s].deadCount++;
                }
            }
            //her briinin altına coun ekle kaç kiş iyileşti hastlandı ve duruma göre özellikleri pasif veya aktif et.
            if (allDataForEntities[s].Invisible == false) {
                if (allDataForEntities[s].incubationCount == 3 && allDataForEntities[s].isEntityInfected == false && allDataForEntities[s].recovered == false && allDataForEntities[s].dead == false) {
                    allDataForEntities[s].isEntityInfected = true;
                    allDataForEntities[s].isInIncubationTime = false;
                }
                else if (allDataForEntities[s].recoveredCount == 30 && allDataForEntities[s].recovered == false && allDataForEntities[s].immune == false && allDataForEntities[s].dead == false) {
                    allDataForEntities[s].isEntityInfected = false;
                    allDataForEntities[s].immune = true;
                    allDataForEntities[s].recovered = true;
                }
                else if (allDataForEntities[s].deadCount == 14 && allDataForEntities[s].dead == false && allDataForEntities[s].immune == false && allDataForEntities[s].cantMove==false) {
                    allDataForEntities[s].dead = true;
                    allDataForEntities[s].cantMove = true;

                }
                else if (allDataForEntities[s].removeDeadCount == 5 && allDataForEntities[s].dead == true) {
                    allDataForEntities[s].Invisible = true;
                    allDataForEntities[s].dead = false;
                    allDataForEntities[s].isEntityInfected = false;
                }
            }    
        }
        //Her bir turn'de neleri kontrol edeceğimi ve ne gibi atamalar yapacağımı kontrol ediyorum.
        for (int k = 0; k < entity_count; k++) {
            if (allDataForEntities[k].cantMove == false && allDataForEntities[k].Invisible == false) {
                //Default locaitonu O andaki turndeki locaitona atıyoruz. 
                allDataForEntities[k].currentLocation[0] = stoi(turns[i][k + 2].substr(0, turns[i][k + 2].find('x')));
                allDataForEntities[k].currentLocation[1] = stoi(turns[i][k + 2].substr(turns[i][k + 2].find('x') + 1, turns[i][k + 2].size() - 1));
            }
        }
        //Turn txt sini burada işliyoruz.
        bool cont = false;
        output2 << "Turn " << i + 1 << ":"<< endl;
        for (int x = 0; x <= size + 1; x++) {
            for (int y = 0; y <= size + 1; y++) {
                if ((x > 0 && x < size + 1) && (y > 0 && y < size + 1)) {
                    for (int j = 0; j < entity_count; j++) {
                        if (allDataForEntities[j].currentLocation[0] == x && allDataForEntities[j].currentLocation[1] == y) {
                            if (allDataForEntities[j].isEntityInfected && allDataForEntities[j].dead==false || allDataForEntities[j].isInIncubationTime) {
                                output2 << "X";
                            }
                            else if (allDataForEntities[j].dead) {
                                output2 << "D";
                            }
                            else if (allDataForEntities[j].recovered && allDataForEntities[j].dead==false && allDataForEntities[j].isEntityInfected==false) {
                                output2 << "0";
                            }
                            else if (allDataForEntities[j].Invisible) {
                                continue;                                
                            }
                            else {
                                output2 << "O";
                            }
                            cont = true;
                        }
                    }         
                    if(cont == false)  {
                        output2 << " ";
                    }
                    else if (cont) {
                        cont = false;
                    }
                }
                else {
                    output2 << "-";
                }
            }
            output2 << endl;
        }  
    }
    output2.close();
    
    for (int k = 0; k < entity_count; k++) {
        if (allDataForEntities[k].dead || allDataForEntities[k].Invisible) {
            deadcount++;
        }
        else if (allDataForEntities[k].isEntityInfected || allDataForEntities[k].isInIncubationTime) {
            infectedcount++;
        }
        else if (allDataForEntities[k].recovered ) {
            recoveredCount++;
        }
    } 
    normalCount = entity_count - infectedcount - deadcount - recoveredCount;
    OkumaDosyasi2.close();

    ofstream output1;
    output1.open("output.txt");
    if (output1.is_open()) {
        output1 << "Normal   : " << normalCount << endl;
        output1 << "Infected : " << infectedcount << endl;
        output1 << "Dead     : " << deadcount << endl;
        output1 << "Recovered: " << recoveredCount << endl;
    }
    
    for (int i = 0; i < entity_count; i++) {
        output1 << "entity " + allDataForEntities[i].name;
        output1 << " " << allDataForEntities[i].currentLocation[0] << "x" << allDataForEntities[i].currentLocation[1];
        output1 << " ";
        if (allDataForEntities[i].Invisible || allDataForEntities[i].dead) {
            output1 << "dead" << endl;
        }
        else if (allDataForEntities[i].isEntityInfected) {
            output1 << "infected" << endl;
        }
        else if (allDataForEntities[i].recovered) {
            output1 << "immune" << endl;
        }
        else {
            output1 << "normal" << endl;
        }
    }
    output1.close(); 
    return 0;
};