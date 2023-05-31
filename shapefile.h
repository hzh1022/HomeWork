#ifndef SHAPEFILE_H
#define SHAPEFILE_H


#include "shape.h"
#include "shapefile.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <QString>
#include <qpoint.h>


using std::string;
using std::vector;
using std::ostringstream;
using std::ios_base;
using std::setiosflags;
using std::setprecision;
using std::ifstream;
using std::ios;
using std::ostream;

struct BBox {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

struct Point {
    double x;
    double y;
};

class Geometry {
public:
    virtual QVector<Info>  wkt() const = 0;

    virtual void Build(char *buf, int size) = 0;

protected:
    vector<vector<Point>> geom;
};

class CPoint : public Geometry {
public:
    virtual QVector<Info>  wkt() const {Info info; return QVector<Info>(); };

    virtual void Build(char *buf, int size) {};
};

class CPolyline : public Geometry {
public:
    virtual QVector<Info>  wkt() const {
        QVector<Info>  info;
        if (geom.size() == 1) {
            //LINESTRING(3 4, 10 50, 20 25)
            Info t;
            strcpy(t.type,"折线");
            for (int i = 0; i < geom[0].size(); i++) {
                t.points.push_back(QPointF(geom[0][i].x ,geom[0][i].y));
            }
            t.len = t.points.size();
            if(t.points.size()>=2)
                info.push_back(t);
            return info;
        } else {
            //MULTILINESTRING((3 4,10 50,20 25),(-5 -8,-10 -8,-15 -4))
            return info;
        }
    };

    virtual void Build(char *buf, int size) {
        int numParts = *(int *) (buf + 36);
        int numPoints = *(int *) (buf + 40);
        int *Parts = (int *) (buf + 44);
        Point *pt = (Point *) (buf + 44 + 4 * numParts);

        for (int i = 0; i < numParts; i++) {
            int nextPart = numPoints;
            if (i < numParts - 1)
                nextPart = Parts[i + 1];

            vector<Point> line;
            for (int j = Parts[i]; j < nextPart; j++)
                line.push_back(pt[j]);

            geom.push_back(line);
        }
    }
};

class CPolygon : public CPolyline {
public:
    virtual QVector<Info>  wkt() const {
        //POLYGON((1 1,5 1,5 5,1 5,1 1),(2 2,2 3,3 3,3 2,2 2))

        QVector<Info>  info;

        //LINESTRING(3 4, 10 50, 20 25)
        for (int i = 0; i < geom.size(); i++) {
            Info t;
            strcpy(t.type,"多边形");
            for (int j = 0; j < geom[i].size(); j++) {
                t.points.push_back(QPointF(geom[i][j].x ,geom[i][j].y));
            }
            t.len = t.points.size();
            if(t.points.size()>2)
                info.push_back(t);
        }
        return info;
    };
};

class Shapefile {
    ifstream ifs;
public:
    bool Open(const char *fname);

    int GetShapeType();

    string GetShapeTypeString() { return ""; }

    bool GetBoundingBox(BBox &box);


    class iterator {
        ifstream &ifs;
        int off;
    public:
        iterator(ifstream &ifs, int off) : ifs(ifs), off(off) {};
        bool operator <(const iterator& d)
        {
            if(off < d.off)
            {
                return true;
            }
            return false;
        }
        void operator++(int) {
            ifs.seekg(off + 4);
            int size = GetBigInt() * 2;

            off += 8;
            off += size;
        };

        Geometry *operator*() {
            ifs.seekg(off + 8);
            int type = GetLittleInt();
            Geometry *pt = NULL;
            if (type == 1)
                pt = new CPoint;//1  1
            else if (type == 3)
                pt = new CPolyline;//n ,n0,n1....
            else if (type == 5)
                pt = new CPolygon;//n,n0,n2

            ifs.seekg(off + 4);
            int size = GetBigInt() * 2;
            char *ptBuf = new char[size];
            ifs.seekg(off + 8);
            ifs.read(ptBuf, size);

            pt->Build(ptBuf, size);

            delete[]ptBuf;

            return pt;
        };

        bool operator!=(iterator &oth) {
            return off != oth.off;
        };

    private:
        int GetBigInt() {
            //cc

            //return (i >> 24 & 0xff) | (i >> 8 & 0xff00) | (i << 8 & 0xff0000) | (i << 24);

            char tmp[4], x[4];
            ifs.read(tmp, sizeof(int));
            x[0] = tmp[3];
            x[1] = tmp[2];
            x[2] = tmp[1];
            x[3] = tmp[0];
            return *(int *) x;
        }

        int GetLittleInt() {
            int i;
            ifs.read((char *) &i, sizeof(int));
            return i;
        }

        double GetDouble() {
            double d;
            ifs.read((char *) &d, sizeof(double));
            return d;
        }
    };

    iterator begin() { return iterator(ifs, 100); };

    iterator end() {
        ifs.seekg(0, ios::end);
        int size0 = ifs.tellg();
        return iterator(ifs, size0);
    };

private:
    int GetBigInt() {
        //cc

        //return (i >> 24 & 0xff) | (i >> 8 & 0xff00) | (i << 8 & 0xff0000) | (i << 24);

        char tmp[4], x[4];
        ifs.read(tmp, sizeof(int));
        x[0] = tmp[3];
        x[1] = tmp[2];
        x[2] = tmp[1];
        x[3] = tmp[0];
        return *(int *) x;
    }

    int GetLittleInt() {
        int i;
        ifs.read((char *) &i, sizeof(int));
        return i;
    }

    double GetDouble() {
        double d;
        ifs.read((char *) &d, sizeof(double));
        return d;
    }
};


int load(QString name);


#endif // SHAPEFILE_H
