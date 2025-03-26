use reqwest::blocking::Client;
use reqwest::header::CONTENT_TYPE;
use dotenvy::dotenv;
use std::env;
use base64::engine::general_purpose;
use base64::Engine; 
use byteorder::{ByteOrder, LittleEndian, BigEndian};
use std::error::Error;

const BASE_URL: &str = "https://hackattic.com/challenges/help_me_unpack";

fn main() -> Result<(), Box<dyn Error>> {
    dotenv().ok();
    let access_token = env::var("ACCESS_TOKEN")
                                .expect("ACCESS_TOKEN is not set");

    let client = Client::new();
    let problem_url = format!("{}/problem?access_token={}", BASE_URL, access_token);
    let response_text = client.get(&problem_url).send()?.text()?;     
    let response: serde_json::Value = serde_json::from_str(&response_text)?;
    
    let encoded_bytes = response["bytes"].as_str().ok_or("Missing 'bytes' field")?;
    let decoded_bytes = general_purpose::STANDARD.decode(encoded_bytes)?;

    println!("Encoded bytes: {}", encoded_bytes);
    println!("Decoded bytes: {:?}", decoded_bytes);

    if decoded_bytes.len() < 30 {
        return Err("Insufficient bytes received".into());
    }

    let int_val: i32 = LittleEndian::read_i32(&decoded_bytes[0..4]);
    let uint_val: u32 = LittleEndian::read_u32(&decoded_bytes[4..8]);
    let short_val: i16 = LittleEndian::read_i16(&decoded_bytes[8..10]);
    let float_val: f32 = LittleEndian::read_f32(&decoded_bytes[12..16]);
    let double_val: f64 = LittleEndian::read_f64(&decoded_bytes[16..24]);
    let big_endian_double_val: f64 = BigEndian::read_f64(&decoded_bytes[24..32]);
    
    let solution = serde_json::json!({
        "int": int_val,
        "uint": uint_val,
        "short": short_val,
        "float": float_val,
        "double": double_val,
        "big_endian_double": big_endian_double_val
    });

    println!("Solution: {:?}", solution);

    let solution_url = format!("{}/solve?access_token={}&playground=1", BASE_URL, access_token);
    let response = client.post(&solution_url)
        .header(CONTENT_TYPE, "application/json")
        .json(&solution)
        .send()?;

    println!("Response: {:?}", response.text()?);
    Ok(())
}
