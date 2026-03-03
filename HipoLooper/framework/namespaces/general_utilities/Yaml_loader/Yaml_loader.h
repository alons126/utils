//
// Created by Alon Sportes on 25/02/2025.
//

// #pragma once

#include <string>

#include "../../../structures/DataSample/DataSample.h"

namespace Yaml_loader {

// Load from a YAML file path (throws std::runtime_error on errors).
DataSample LoadFromYamlFile(const std::string& yaml_path);

}  // namespace Yaml_loader
