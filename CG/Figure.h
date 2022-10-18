#pragma once
#include <vector>
#include "Primitives.h"
#include <functional>
/*
Immortal temptation
Takes over my mind, condemned
Falling weak on my knees
Summon the strength of mayhem

I am the storm that is approaching
Provoking black clouds in isolation
I am reclaimer of my name
Born in flames, I have been blessed
My family crest is a demon of death
Forsakened, I am awakened
A phoenix's ash in dark divine
Descending misery
Destiny chasing time

Inherit the nightmare, surrounded by fate
Can't run away
Keep walking the line, between the light
Led astray

Through vacant halls I won't surrender
The truth revealed in eyes of ember
We fight through fire and ice forever
Two souls once lost, and now they remember

I am the storm that is approaching
Provoking black clouds in isolation
I am reclaimer of my name
Born in flames, I have been blessed
My family crest is a demon of death
Forsakened, I am awakened
A phoenix's ash in dark divine
Descending misery
Destiny chasing time

Disappear into the night
Lost shadows left behind
Obsession's pulling me
Fading, I've come to take what's mine

Lurking in the shadows under veil of night
Constellations of blood pirouette
Dancing through the graves of those who stand at my feet
Dreams of the black throne I keep on repeat
A derelict of dark, summoned from the ashes
The puppet master congregates all the masses
Pulling strings, twisting minds as blades hit
You want this power? Then come try and take it

Beyond the tree, fire burns
Secret love, bloodline yearns
Dark minds embrace, crimson joy
Does your dim heart heal or destroy?

Bury the light deep within
Cast aside, there's no coming home
We're burning chaos in the wind
Drifting in the ocean all alone
*/


/// <summary>
/// ...и тут товарищ майор, вы не поверите,
/// вы€сн€етс€, что грань фигуры - это фэйс
/// </summary>
class Face {
public:
    std::vector<Edge> edges;
    Face() {
        edges = std::vector<Edge>();
    }

    glm::vec3 center() {
        float x=0, y=0, z=0;
        for (auto edge : edges) {
            x += edge.points[0].x;
            y += edge.points[0].y;
            z += edge.points[0].z;
        }
        size_t size = edges.size();
        x /= size;
        y /= size;
        z /= size;
        return glm::vec3(x, y, z);
    }
};
/// <summary>
/// Ќу, типа, фигура, да
/// </summary>
class Figure {
public:
    /// <summary>
    /// √л€ньте на него, каков лицемер!
    /// </summary>
    std::vector<Face> faces;

    Figure() {
        faces = std::vector<Face>();
    }

    /// <summary>
    /// “рансформаторна€ будка
    /// ту-ту-ту
    /// </summary>
    void transform(std::function<glm::vec3(glm::vec3)> transofrmator) { //TODO: если найдЄшь, как передавать по ссылке аргумент - молодец, помен€й
        for (auto i = 0; i < faces.size(); i++) {
            for (auto j = 0; j < faces[j].edges.size(); i++) {
                Edge* edge = &(faces[i].edges[j]);
                edge->points[0] = transofrmator(edge->points[0]);
                edge->points[1] = transofrmator(edge->points[1]);
            }
        }
    }
};


class Tetrahedron : public Figure {};

class Octahedron : public Figure {};

class Hexahedron : public Figure {};

class Icosahedron : public Figure {};

class Dodecahedron : public Figure {};

enum FigureType {
    Tetrahed, Octahed,
    Hexahed, Icosahed,
    Dodecahed
};

class FigureBuilder {
public:
    Figure buildFigure(FigureType kind) {
        switch (kind){
        case Tetrahed:
            return buildTetrahedron();
        case Hexahed:
            return buildHexahedron();
        case Octahed:
            return buildOctahedron();
        case Icosahed:
            break;
        case Dodecahed:
            break;
        }
    }

    Tetrahedron buildTetrahedron() {
        auto tetr = Tetrahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 c = glm::vec3(200, 0, 200);
        glm::vec3 f = glm::vec3(200, 200, 0);
        glm::vec3 h = glm::vec3(0, 200, 200);
        Face face1 = Face();
        //TODO: изобрести метод buildTriangle
        {
            Edge edge1 = Edge(a, glm::vec3(0, 0, 0));
            edge1.push_point(f);
            Edge edge2 = Edge(f, glm::vec3(0, 0, 0));
            edge2.push_point(c);
            Edge edge3 = Edge(c, glm::vec3(0, 0, 0));
            edge3.push_point(a);
            face1.edges.push_back(edge1);
            face1.edges.push_back(edge2);
            face1.edges.push_back(edge3);
        }
        Face face2 = Face();
        {
            Edge edge1 = Edge(f, glm::vec3(0, 0, 0));
            edge1.push_point(c);
            Edge edge2 = Edge(c, glm::vec3(0, 0, 0));
            edge2.push_point(h);
            Edge edge3 = Edge(h, glm::vec3(0, 0, 0));
            edge3.push_point(f);
            face2.edges.push_back(edge1);
            face2.edges.push_back(edge2);
            face2.edges.push_back(edge3);
        }
        Face face3 = Face();
        {
            Edge edge1 = Edge(c, glm::vec3(0, 0, 0));
            edge1.push_point(h);
            Edge edge2 = Edge(h, glm::vec3(0, 0, 0));
            edge2.push_point(a);
            Edge edge3 = Edge(a, glm::vec3(0, 0, 0));
            edge3.push_point(c);
            face3.edges.push_back(edge1);
            face3.edges.push_back(edge2);
            face3.edges.push_back(edge3);
        }
        Face face4 = Face();
        {
            Edge edge1 = Edge(f, glm::vec3(0, 0, 0));
            edge1.push_point(h);
            Edge edge2 = Edge(h, glm::vec3(0, 0, 0));
            edge2.push_point(a);
            Edge edge3 = Edge(a, glm::vec3(0, 0, 0));
            edge3.push_point(f);
            face4.edges.push_back(edge1);
            face4.edges.push_back(edge2);
            face4.edges.push_back(edge3);
        }
        tetr.faces.push_back(face1);
        tetr.faces.push_back(face2);
        tetr.faces.push_back(face3);
        tetr.faces.push_back(face4);
        return tetr;

    }

    Hexahedron buildHexahedron() {
        auto hexa = Hexahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 b = glm::vec3(200, 0, 0);
        glm::vec3 c = glm::vec3(200, 0, 200);
        glm::vec3 d = glm::vec3(0, 0, 200);
        glm::vec3 e = glm::vec3(0, 200, 0);
        glm::vec3 f = glm::vec3(200, 200, 0);
        glm::vec3 g = glm::vec3(200, 200, 200);
        glm::vec3 h = glm::vec3(0, 200, 200);

        Face face1 = Face();
        //TODO: изобрести метод buildSqare
        {
            Edge edge1 = Edge(a, glm::vec3(0, 0, 0));
            edge1.push_point(b);
            Edge edge2 = Edge(b, glm::vec3(0, 0, 0));
            edge2.push_point(c);
            Edge edge3 = Edge(c, glm::vec3(0, 0, 0));
            edge3.push_point(d);
            Edge edge4 = Edge(d, glm::vec3(0, 0, 0));
            edge4.push_point(a);
            face1.edges.push_back(edge1);
            face1.edges.push_back(edge2);
            face1.edges.push_back(edge3);
            face1.edges.push_back(edge4);
        }
        Face face2 = Face();
        {
            Edge edge1 = Edge(b, glm::vec3(0, 0, 0));
            edge1.push_point(c);
            Edge edge2 = Edge(c, glm::vec3(0, 0, 0));
            edge2.push_point(g);
            Edge edge3 = Edge(g, glm::vec3(0, 0, 0));
            edge3.push_point(f);
            Edge edge4 = Edge(f, glm::vec3(0, 0, 0));
            edge4.push_point(b);
            face2.edges.push_back(edge1);
            face2.edges.push_back(edge2);
            face2.edges.push_back(edge3);
            face2.edges.push_back(edge4);
        }
        Face face3 = Face();
        {
            Edge edge1 = Edge(f, glm::vec3(0, 0, 0));
            edge1.push_point(g);
            Edge edge2 = Edge(g, glm::vec3(0, 0, 0));
            edge2.push_point(h);
            Edge edge3 = Edge(h, glm::vec3(0, 0, 0));
            edge3.push_point(e);
            Edge edge4 = Edge(e, glm::vec3(0, 0, 0));
            edge4.push_point(f);
            face3.edges.push_back(edge1);
            face3.edges.push_back(edge2);
            face3.edges.push_back(edge3);
            face3.edges.push_back(edge4);
        }
        Face face4 = Face();
        {
            Edge edge1 = Edge(h, glm::vec3(0, 0, 0));
            edge1.push_point(e);
            Edge edge2 = Edge(e, glm::vec3(0, 0, 0));
            edge2.push_point(a);
            Edge edge3 = Edge(a, glm::vec3(0, 0, 0));
            edge3.push_point(d);
            Edge edge4 = Edge(d, glm::vec3(0, 0, 0));
            edge4.push_point(h);
            face4.edges.push_back(edge1);
            face4.edges.push_back(edge2);
            face4.edges.push_back(edge3);
            face4.edges.push_back(edge4);
        }
        Face face5 = Face();
        {
            Edge edge1 = Edge(a, glm::vec3(0, 0, 0));
            edge1.push_point(b);
            Edge edge2 = Edge(b, glm::vec3(0, 0, 0));
            edge2.push_point(f);
            Edge edge3 = Edge(f, glm::vec3(0, 0, 0));
            edge3.push_point(e);
            Edge edge4 = Edge(e, glm::vec3(0, 0, 0));
            edge4.push_point(a);
            face1.edges.push_back(edge1);
            face1.edges.push_back(edge2);
            face1.edges.push_back(edge3);
            face1.edges.push_back(edge4);
        }
        Face face6 = Face();
        {
            Edge edge1 = Edge(d, glm::vec3(0, 0, 0));
            edge1.push_point(c);
            Edge edge2 = Edge(c, glm::vec3(0, 0, 0));
            edge2.push_point(g);
            Edge edge3 = Edge(g, glm::vec3(0, 0, 0));
            edge3.push_point(h);
            Edge edge4 = Edge(h, glm::vec3(0, 0, 0));
            edge4.push_point(d);
            face1.edges.push_back(edge1);
            face1.edges.push_back(edge2);
            face1.edges.push_back(edge3);
            face1.edges.push_back(edge4);
        }
        hexa.faces.push_back(face1);
        hexa.faces.push_back(face2);
        hexa.faces.push_back(face3);
        hexa.faces.push_back(face4);
        hexa.faces.push_back(face5);
        hexa.faces.push_back(face6);

        return hexa;
    }

    Octahedron buildOctahedron() {
        auto res = Octahedron();
        auto core = buildHexahedron();
        auto a = core.faces[0].center();
        auto b = core.faces[1].center();
        auto c = core.faces[2].center();
        auto d = core.faces[3].center();
        auto e = core.faces[4].center();
        auto f = core.faces[5].center();

        Face face1 = Face();
        {
            Edge edge1 = Edge(a, glm::vec3(0, 0, 0));
            edge1.push_point(f);
            Edge edge2 = Edge(f, glm::vec3(0, 0, 0));
            edge2.push_point(b);
            Edge edge3 = Edge(b, glm::vec3(0, 0, 0));
            edge3.push_point(a);
            face1.edges.push_back(edge1);
            face1.edges.push_back(edge2);
            face1.edges.push_back(edge3);
        }
        Face face2 = Face();
        {
            Edge edge1 = Edge(b, glm::vec3(0, 0, 0));
            edge1.push_point(c);
            Edge edge2 = Edge(c, glm::vec3(0, 0, 0));
            edge2.push_point(f);
            Edge edge3 = Edge(f, glm::vec3(0, 0, 0));
            edge3.push_point(b);
            face2.edges.push_back(edge1);
            face2.edges.push_back(edge2);
            face2.edges.push_back(edge3);
        }
        Face face3 = Face();
        {
            Edge edge1 = Edge(c, glm::vec3(0, 0, 0));
            edge1.push_point(d);
            Edge edge2 = Edge(d, glm::vec3(0, 0, 0));
            edge2.push_point(f);
            Edge edge3 = Edge(f, glm::vec3(0, 0, 0));
            edge3.push_point(c);
            face3.edges.push_back(edge1);
            face3.edges.push_back(edge2);
            face3.edges.push_back(edge3);
        }
        Face face4 = Face();
        {
            Edge edge1 = Edge(d, glm::vec3(0, 0, 0));
            edge1.push_point(a);
            Edge edge2 = Edge(a, glm::vec3(0, 0, 0));
            edge2.push_point(f);
            Edge edge3 = Edge(f, glm::vec3(0, 0, 0));
            edge3.push_point(d);
            face4.edges.push_back(edge1);
            face4.edges.push_back(edge2);
            face4.edges.push_back(edge3);
        }
        Face face4 = Face();
        {
            Edge edge1 = Edge(a, glm::vec3(0, 0, 0));
            edge1.push_point(e);
            Edge edge2 = Edge(e, glm::vec3(0, 0, 0));
            edge2.push_point(b);
            Edge edge3 = Edge(b, glm::vec3(0, 0, 0));
            edge3.push_point(a);
            face4.edges.push_back(edge1);
            face4.edges.push_back(edge2);
            face4.edges.push_back(edge3);
        }
        Face face5 = Face();
        {
            Edge edge1 = Edge(b, glm::vec3(0, 0, 0));
            edge1.push_point(e);
            Edge edge2 = Edge(e, glm::vec3(0, 0, 0));
            edge2.push_point(c);
            Edge edge3 = Edge(c, glm::vec3(0, 0, 0));
            edge3.push_point(b);
            face5.edges.push_back(edge1);
            face5.edges.push_back(edge2);
            face5.edges.push_back(edge3);
        }
        Face face6 = Face();
        {
            Edge edge1 = Edge(c, glm::vec3(0, 0, 0));
            edge1.push_point(e);
            Edge edge2 = Edge(e, glm::vec3(0, 0, 0));
            edge2.push_point(d);
            Edge edge3 = Edge(d, glm::vec3(0, 0, 0));
            edge3.push_point(c);
            face6.edges.push_back(edge1);
            face6.edges.push_back(edge2);
            face6.edges.push_back(edge3);
        }
        Face face7 = Face();
        {
            Edge edge1 = Edge(d, glm::vec3(0, 0, 0));
            edge1.push_point(e);
            Edge edge2 = Edge(e, glm::vec3(0, 0, 0));
            edge2.push_point(a);
            Edge edge3 = Edge(a, glm::vec3(0, 0, 0));
            edge3.push_point(d);
            face7.edges.push_back(edge1);
            face7.edges.push_back(edge2);
            face7.edges.push_back(edge3);
        }
        res.faces.push_back(face1);
        res.faces.push_back(face2);
        res.faces.push_back(face3);
        res.faces.push_back(face4);
        res.faces.push_back(face5);
        res.faces.push_back(face6);
        res.faces.push_back(face7);
        return res;
    }
};
