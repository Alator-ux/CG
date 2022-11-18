#pragma once
#include "Figure.h"
#include "STLLoader.h"
#include <vector>
class FigureBuilder {
    float bound = 100;
public:
    Figure buildFigure(FigureType kind, glm::vec3 color) {
        Figure fig;
        switch (kind) {
        case Tetrahed:
            fig = buildTetrahedron(color);
            break;
        case Hexahed:
            fig = buildHexahedron(color);
            break;
        case Octahed:
            fig = buildOctahedron(color);
            break;
        case Icosahed:
            fig = buildIcosahedron(color);
            break;
        case Dodecahed:
            fig = buildDodecahedron(color);
            break;
        }
        for (auto i = 0; i < fig.objects.size(); i++) {
            fig.objects[i].set_uv_vec({
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(0, 1) });
        }
        return fig;
    }
    Dodecahedron buildDodecahedron(glm::vec3 color) {
        auto r = STL::load_from_file("./models/ddc.stl");
        Dodecahedron* res = reinterpret_cast<Dodecahedron*>(&r);
        return *res;
    }
    Tetrahedron buildTetrahedron(glm::vec3 color) {
        auto tetr = Tetrahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 c = glm::vec3(bound / 2, 0, -bound / 2);
        glm::vec3 f = glm::vec3(bound / 2, bound / 2, 0);
        glm::vec3 h = glm::vec3(0, bound / 2, -bound / 2);
        a /= bound;
        c /= bound;
        f /= bound;
        h /= bound;

        auto ac = glm::vec3(255.f, 0.f, 0.f);
        auto fc = glm::vec3(0.f, 255.f, 0.f);
        auto cc = glm::vec3(0.f, 0.f, 255.f);
        auto hc = glm::vec3(0.f, 0.f, 0.f);

        Face face1 = Face(a, ac);
        face1.push_point(c, ac);
        face1.push_point(f, ac);
        face1.primitive_is_finished();

        Face face2 = Face(f, ac);
        face2.push_point(c, ac);
        face2.push_point(h, ac);
        face2.primitive_is_finished();

        Face face3 = Face(c, ac);
        face3.push_point(a, ac);
        face3.push_point(h, ac);
        face3.primitive_is_finished();

        Face face4 = Face(f, ac);
        face4.push_point(h, ac);
        face4.push_point(a, ac);
        face4.primitive_is_finished();

        tetr.objects.push_back(face1);
        tetr.objects.push_back(face2);
        tetr.objects.push_back(face3);
        tetr.objects.push_back(face4);
        return tetr;

    }

    Hexahedron buildHexahedron(glm::vec3 color) {
        auto hexa = Hexahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 b = glm::vec3(bound / 2, 0, 0);
        glm::vec3 c = glm::vec3(bound / 2, 0, -bound / 2);
        glm::vec3 d = glm::vec3(0, 0, -bound / 2);
        glm::vec3 e = glm::vec3(0, bound / 2, 0);
        glm::vec3 f = glm::vec3(bound / 2, bound / 2, 0);
        glm::vec3 g = glm::vec3(bound / 2, bound / 2, -bound / 2);
        glm::vec3 h = glm::vec3(0, bound / 2, -bound / 2);
        a /= bound;
        b /= bound;
        c /= bound;
        d /= bound;
        e /= bound;
        f /= bound;
        g /= bound;
        h /= bound;

        Face face1 = Face(a, glm::vec3(1.f, 0.f, 0.f));
        face1.push_point(b);
        face1.push_point(c);
        face1.push_point(d);
        face1.primitive_is_finished();

        Face face2 = Face(b, glm::vec3(0.f, 1.f, 0.f));
        face2.push_point(c);
        face2.push_point(g);
        face2.push_point(f);
        face2.primitive_is_finished();

        Face face3 = Face(f, glm::vec3(0.f, 0.f, 1.f));
        face3.push_point(g);
        face3.push_point(h);
        face3.push_point(e);
        face3.primitive_is_finished();

        Face face4 = Face(h, glm::vec3(1.f, 1.f, 0.f));
        face4.push_point(e);
        face4.push_point(a);
        face4.push_point(d);
        face4.primitive_is_finished();

        Face face5 = Face(a, glm::vec3(1.f, 0.f, 1.f));
        face5.push_point(b);
        face5.push_point(f);
        face5.push_point(e);
        face5.primitive_is_finished();


        Face face6 = Face(d, glm::vec3(0.f, 1.f, 1.f));
        face6.push_point(c);
        face6.push_point(g);
        face6.push_point(h);
        face6.primitive_is_finished();

        hexa.objects.push_back(face1);
        hexa.objects.push_back(face2);
        hexa.objects.push_back(face3);
        hexa.objects.push_back(face4);
        hexa.objects.push_back(face5);
        hexa.objects.push_back(face6);

        return hexa;
    }

    Octahedron buildOctahedron(glm::vec3 color) {
        auto res = Octahedron();
        auto core = buildHexahedron(color);
        Face* face = reinterpret_cast<Face*>(&core.objects[0]);
        auto a = face->center();
        face = reinterpret_cast<Face*>(&core.objects[1]);
        auto b = face->center();
        face = reinterpret_cast<Face*>(&core.objects[2]);
        auto c = face->center();
        face = reinterpret_cast<Face*>(&core.objects[3]);
        auto d = face->center();
        face = reinterpret_cast<Face*>(&core.objects[4]);
        auto e = face->center();
        face = reinterpret_cast<Face*>(&core.objects[5]);
        auto f = face->center();

        Face face1 = Face(a, glm::vec3(1.f, 0.f, 0.f));
        face1.push_point(f);
        face1.push_point(b);
        face1.primitive_is_finished();

        Face face2 = Face(b, glm::vec3(0.f, 1.f, 0.f));
        face2.push_point(c);
        face2.push_point(f);
        face2.primitive_is_finished();

        Face face3 = Face(c, glm::vec3(0.f, 0.f, 1.f));
        face3.push_point(d);
        face3.push_point(f);
        face3.primitive_is_finished();

        Face face4 = Face(d, glm::vec3(1.f, 1.f, 0.f));
        face4.push_point(a);
        face4.push_point(f);
        face4.primitive_is_finished();

        Face face5 = Face(a, glm::vec3(1.f, 0.f, 1.f));
        face5.push_point(e);
        face5.push_point(b);
        face5.primitive_is_finished();

        Face face6 = Face(b, glm::vec3(0.f, 1.f, 1.f));
        face6.push_point(e);
        face6.push_point(c);
        face6.primitive_is_finished();

        Face face7 = Face(c, glm::vec3(1.f, 1.f, 1.f));
        face7.push_point(e);
        face7.push_point(d);
        face7.primitive_is_finished();

        Face face8 = Face(d, glm::vec3(0.5f, 0.5f, 0.5f));
        face8.push_point(e);
        face8.push_point(a);
        face8.primitive_is_finished();

        res.objects.push_back(face1);
        res.objects.push_back(face2);
        res.objects.push_back(face3);
        res.objects.push_back(face4);
        res.objects.push_back(face5);
        res.objects.push_back(face6);
        res.objects.push_back(face7);
        res.objects.push_back(face8);
        return res;
    }

    Icosahedron buildIcosahedron(glm::vec3 color) {
        auto r = STL::load_from_file("./models/icosahedron.stl");
        Icosahedron* res = reinterpret_cast<Icosahedron*>(&r);
        return *res;

    }
};