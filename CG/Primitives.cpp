#include "Primitives.h"
#include <stack>

float rotate(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
}
Polygon graham(std::vector<glm::vec3> points, glm::vec3 color) {
    std::sort(points.begin(), points.end(), [](glm::vec3& p1, glm::vec3& p2) {
        return p1.x < p2.x;
        });
    
    for (size_t i = 2; i < points.size(); i++) {
        size_t j = i;
        while (j > 1 && rotate(points[0], points[j - 1], points[j]) < 0) {
            std::swap(points[j - 1], points[j]);
            j--;
        }
    }

    std::stack<glm::vec3> st;
    st.push(points[0]);
    st.push(points[1]);

    size_t i = 2;
    while (i != points.size()) {
        auto sp = st.top();
        st.pop();
        auto fp = st.top();
        st.push(sp);
        if (rotate(fp, sp, points[i]) < 0.f) {
            st.pop();
        }
        else {
            st.push(points[i]);
            i++;
        }
    }

    Polygon poly = Polygon(st.top(), color);
    st.pop();
    while (!st.empty()) {
        poly.push_point(st.top());
        st.pop();
    }

    return poly;
}
