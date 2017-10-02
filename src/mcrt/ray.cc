#include "mcrt/ray.hh"

#include <glm/gtx/rotate_vector.hpp> 
#include <iostream>
#include <glm/gtc/constants.hpp>

glm::dvec3 mcrt::Ray::Intersection::sampleHemisphere(const Ray& i) const {
    glm::dvec3 v1 = glm::normalize(i.direction - glm::dot(i.direction, normal) * normal);
    glm::dvec3 v2 = glm::normalize(glm::cross(v1,normal));

    //std::cout << v1.x << " " << v1.y << " " << v1.z << std::endl;
   // std::cout << v2.x << " " << v2.y << " " << v2.z << std::endl;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);

    double theta1 = dis(gen) * glm::pi<double>() * 2.0;
    double theta2 = dis(gen) * glm::pi<double>() * 0.5;
    
    const glm::dvec3 azimuthRotation = glm::rotate(v1,theta1,normal);
    const glm::dvec3 inclanationRotation = glm::rotate(v1, theta2,v2);
    const glm::dvec3 outgoing = glm::normalize(azimuthRotation + inclanationRotation);
    return outgoing;
}