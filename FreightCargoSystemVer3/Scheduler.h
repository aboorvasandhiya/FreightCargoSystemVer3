#pragma once
#include <vector>
#include <map>
#include "Freight.h"
#include "Cargo.h"
using namespace std;

class Scheduler {
private:
    vector<Freight> freights;
    vector<Cargo> cargos;
    map<string, string> assignments; // freightID -> cargoID

public:
    void loadFreights(const string& filePath);
    void loadCargos(const string& filePath);
    void generateSchedule();
    void displayUnassigned() const;
    void saveSchedule(const string& filePath) const;

    void addFreight(Freight freight);
    void editFreight(const string& id);
    void deleteFreight(const string& id);

    void addCargo(Cargo cargo);
    void editCargo(const string& id);
    void deleteCargo(const string& id);

    void displaySchedule() const;

    void saveFreights(const string& filePath) const;
    void saveCargos(const string& filePath) const;
    void displayAllFreights() const;
    void displayAllCargos() const;


    void deleteFreightById(const string& id);
    void deleteCargoById(const string& id);

    const vector<Freight>& getFreights() const { return freights; }
    const vector<Cargo>& getCargos() const { return cargos; }

};
