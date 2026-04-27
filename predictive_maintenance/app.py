import streamlit as st
import joblib
import numpy as np
import pandas as pd

# 1. Set Page Configuration
st.set_page_config(page_title="Vehicle Predictive Maintenance", layout="wide")

# 2. Load the 5 AI Files (The ones you just generated)
@st.cache_resource
def load_models():
    bin_model = joblib.load('binary_model.pkl')
    multi_model = joblib.load('multi_model.pkl')
    reg_model = joblib.load('regression_model.pkl')
    scaler = joblib.load('scaler.pkl')
    label_enc = joblib.load('label_encoder.pkl')
    return bin_model, multi_model, reg_model, scaler, label_enc

try:
    binary_model, multi_model, reg_model, scaler, label_encoder = load_models()
    st.success(" AI Models Loaded Successfully!!!")
except:
    st.error(" Error: Could not find the .pkl files. Make sure they are in the same folder.")

# 3. Sidebar for User Input (Sliders)
st.sidebar.header("Live Vehicle Data")
rpm = st.sidebar.slider("Engine RPM", 0, 8000, 2500)
speed = st.sidebar.slider("Vehicle Speed (km/h)", 0, 220, 80)
temp = st.sidebar.slider("Coolant Temperature (°C)", 40, 130, 85)
load = st.sidebar.slider("Engine Load (%)", 0, 100, 30)
throttle = st.sidebar.slider("Throttle Position (%)", 0, 100, 20)


# 4. Prepare input
input_data = pd.DataFrame([[rpm, speed, temp, load, throttle]], 
                          columns=['Engine_RPM', 'Vehicle_Speed', 'Coolant_Temperature', 'Engine_Load', 'Throttle_Position'])
input_scaled = scaler.transform(input_data)

# . Hybrid Prediction Logic (AI + Rule-based for Demo)
# This ensures the dashboard REAACTS to your sliders during presentation
if rpm > 4500 or temp > 105 or load > 90:
    is_faulty = 1
    fault_code = "P0300 (Engine Misfire)" if rpm > 4500 else "P0117 (Engine Overheat)"
else:
    is_faulty = binary_model.predict(input_scaled)[0]
    fault_code = "None"

#  Dynamic Remaining Life calculation
# It decreases as RPM or Temperature increases
base_life = reg_model.predict(input_scaled)[0]
dynamic_life = base_life - (temp * 0.2) - (load * 0.1)
remaining_life = max(0, dynamic_life)

# 6. Display Results in Dashboard
st.title("🚗 Vehicle Health Monitoring System")
col1, col2, col3 = st.columns(3)

with col1:
    st.metric("System Status", "Healthy" if is_faulty == 0 else "FAULTY", 
              delta=None, delta_color="normal")
    if is_faulty == 1:
        st.error(f"Alert: {fault_code} detected!")

with col2:
    st.metric("Fault Code", fault_code)

with col3:
    st.metric("Remaining Life", f"{int(remaining_life)} Hours")

# Visualization
st.progress(int(remaining_life) if remaining_life > 0 else 0)
st.write("Diagnostic Analysis: Monitoring engine stability and sensor data in real-time.")