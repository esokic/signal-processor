#include "exportfilesetting.h"

ExportFileSetting::ExportFileSetting(QObject *parent) : QObject(parent)
{

}


void ExportFileSetting::izbrisiElementIzVektoraSignalaZaEksport(ulong currentRow)
{
    if (currentRow < listaSignalaZaExport.size())
    {
        listaSignalaZaExport.erase(listaSignalaZaExport.begin() + currentRow);
    }
}

void ExportFileSetting::shiftUpElementIzVektoraSignalaZaEksport(ulong currentRow)
{
    if (currentRow > 0 && currentRow < listaSignalaZaExport.size()) {
        std::swap(listaSignalaZaExport[currentRow], listaSignalaZaExport[currentRow - 1]);
    }
}
void ExportFileSetting::shiftDownElementIzVektoraSignalaZaEksport(ulong currentRow)
{
    if (currentRow < listaSignalaZaExport.size() - 1) {
        std::swap(listaSignalaZaExport[currentRow], listaSignalaZaExport[currentRow + 1]);
    }
}
