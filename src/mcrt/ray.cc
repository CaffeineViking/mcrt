#include "mcrt/ray.hh"

#include <iostream>

glm::dvec3 mcrt::Ray::Intersection::sampleHemisphere(const Ray& i) const {
    glm::dvec3 v1 = glm::normalize(i.direction - glm::dot(i.direction, normal) * normal);
    glm::dvec3 v2 = glm::normalize(glm::cross(v1,normal));

    //std::cout << v1.x << " " << v1.y << " " << v1.z << std::endl;
   // std::cout << v2.x << " " << v2.y << " " << v2.z << std::endl;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);

    double theta1 = dis(gen) * M_PI * 2.0;
    double theta2 = dis(gen) * M_PI * 0.5;
    
    //std::cout << theta1 << " " << theta2 << std::endl;
    const glm::dmat3 yRotate 
        {
            std::cos(theta1), 0.0, -std::sin(theta1),
            0.0,             1.0,  0.0,
            std::sin(theta1), 0.0, std::cos(theta1)
        };
    const glm::dmat3 zRotate 
        {
            std::cos(theta2), std::sin(theta2), 0.0,
            -std::sin(theta2), std::cos(theta2), 0.0,
            0.0, 0.0, 1.0
        };

    const glm::dvec3 outgoing = glm::normalize(v1 * yRotate + v1 * zRotate);
    return outgoing;
}