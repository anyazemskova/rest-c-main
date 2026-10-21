#pragma once
// ============================================================================
// ВАРИАНТ 17: Оценщик профессиональных рисков и страховых тарифов
//
// ВАЖНО: имя namespace должно остаться TaskVariant — так работает task.hpp.
// ============================================================================

#include <string>
#include <cmath>
#include "../../json.hpp"

namespace TaskVariant {

 struct Input {
        double base_insurance_rate;       
        int total_employees;    
        int hazardous_conditions_count;            
        int incidents_past_year;     
    };

  struct Output {
        double risk_multiplier;                  
        double final_premium_per_employee;  
        double total_annual_insurance_cost;       
    };

    inline std::string validate(const Input& in) {
        if (in.base_insurance_rate <= 0.0)
            return "base_insurance_rate must be > 0.0";
        if (in.total_employees < 1)
            return "total_employees must be >= 1";
        if (in.hazardous_conditions_count < 0 || in.hazardous_conditions_count>in.total_employees)
            return "hazardous_conditions_count must be in [0, total_employees]";
        if (in.incidents_past_year < 0 || in.incidents_past_year > in.total_employees*3)
            return "incidents_past_year must be in [0, total_employees*3]";
        return "";
    }

    inline Output calculate(const Input& in) 
    {
    Output out{};

    out.risk_multiplier =
        1.0
        + ((double)in.hazardous_conditions_count / (double)in.total_employees) * 0.8
        + ((double)in.incidents_past_year / (double)in.total_employees) * 1.2;

    out.final_premium_per_employee = in.base_insurance_rate * out.risk_multiplier;
    out.total_annual_insurance_cost = (double)in.total_employees * out.final_premium_per_employee;

    return out;
    }

   inline Input parse(const std::string& body) {
        Input in{};

        auto get_d = [&](const std::string& key) -> double {
            auto p = body.find("\"" + key + "\"");
            if (p == std::string::npos) return 0.0;
            p = body.find(':', p);
            if (p == std::string::npos) return 0.0;
            try { return std::stod(body.substr(p + 1)); }
            catch (...) { return 0.0; }
        };

        in.base_insurance_rate    = get_d("base_insurance_rate");
        in.total_employees = (int)get_d("total_employees");
        in.hazardous_conditions_count = (int)get_d("hazardous_conditions_count");
        in.incidents_past_year = (int)get_d("incidents_past_year");
        
        return in;
    }

     inline std::string to_json(const Output& out) {
        JObj o;
        o.num("risk_multiplier", out.risk_multiplier);
        o.num("final_premium_per_employee", out.final_premium_per_employee);
        o.num("total_annual_insurance_cost", out.total_annual_insurance_cost);
        return o.str();
    }

} // namespace TaskVariant

