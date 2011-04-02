#ifndef GLOBALCONST_H
#define GLOBALCONST_H
class ModelParam {
public:
    int N_tanks;    // Всего танков
    int N_killed_by_10th_pturs;    // Убито танков 10 мсб ПТУРов
    int N_killed_by_12th_pturs;     // Убито танков 12 мсб ПТУРов
    int N_artilery; // количество орудий в батарее
    int N_refusal; // отказываются ли танки
    int tanks_direction; // куда идут танки танки
    int ptur_position; // где стоят ПТУРы

    int TON_type;
    bool tankSurrender;
    int tankSurrenderAt;
    int tankPlatoonCount;
    int pturPlatoonCount;
    int d30PlatoonCount;


public:
    ModelParam(){
      N_tanks = 54;
      N_killed_by_10th_pturs = 14;
      N_killed_by_12th_pturs = 20;
      N_artilery = 36;
      N_refusal = 19;
      tanks_direction = 1;
      ptur_position = 1;
    };
    int get_N_refusal() { return N_refusal; };

};

#endif // GLOBALCONST_H
