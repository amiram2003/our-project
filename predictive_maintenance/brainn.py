import pandas as pd
import joblib
import numpy as np
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.ensemble import RandomForestClassifier, RandomForestRegressor

def train_vehicle_system(file_path):
    print("--- Starting AI Training Process ---")
    
    # 1. Load the dataset
    df = pd.read_csv(file_path)
    
   

    df = df.dropna(subset=['Engine RPM', 'Speed (km/h)'])  # FIX: Remove any rows that have missing values (NaN) ---
    
    # 2. Feature Selection
    X = pd.DataFrame()
    X['Engine_RPM'] = df['Engine RPM']
    X['Vehicle_Speed'] = df['Speed (km/h)']
    
    # Fill remaining columns with default values
    X['Coolant_Temperature'] = 85.0
    X['Engine_Load'] = 30.0
    X['Throttle_Position'] = 20.0

    # 3. Target Generation
    # Target 1: Binary
    y_binary = X['Engine_RPM'].apply(lambda x: 1 if x > 4500 else 0)
    
    # Target 2: Multi-class
    y_multi_text = y_binary.apply(lambda x: "P0300" if x == 1 else "None")
    label_enc = LabelEncoder()
    y_multi = label_enc.fit_transform(y_multi_text)
    
    # Target 3: Regression (Remaining life)
    y_remaining_life = 100 - (X['Engine_RPM'] * 0.01)

    # 4. Data Scaling
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # 5. Model Training
    print("Training models... please wait.")
    binary_model = RandomForestClassifier(n_estimators=50).fit(X_scaled, y_binary)
    multi_model = RandomForestClassifier(n_estimators=50).fit(X_scaled, y_multi)
    reg_model = RandomForestRegressor(n_estimators=50).fit(X_scaled, y_remaining_life)

    # 6. Save the 5 required files
    joblib.dump(binary_model, 'binary_model.pkl')
    joblib.dump(multi_model, 'multi_model.pkl')
    joblib.dump(reg_model, 'regression_model.pkl')
    joblib.dump(scaler, 'scaler.pkl')
    joblib.dump(label_enc, 'label_encoder.pkl')
    
    print("---  SUCCESS: All 5 files generated! ---")

# RUN THE TRAINING
train_vehicle_system('separated data.csv')