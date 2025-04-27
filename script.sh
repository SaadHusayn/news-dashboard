curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=AIzaSyDSdd3ifQB0lBfsQVgqtiDSc6CiP57PDZM" -H 'Content-Type: application/json' -X POST -d '{
  "contents": [{
    "parts":[{"text": "generate 60 sentences about current world news and write them line by line in plaintext and dont add numbering to the sentences. Each news sentence should be different in terms of content and shouldnot be too generic. Dont generate intro and outro text, just generate sentences, else no content"}]
    }]
   }' | jq -r '.candidates[0].content.parts[0].text' > news.txt


curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=AIzaSyDSdd3ifQB0lBfsQVgqtiDSc6CiP57PDZM" -H 'Content-Type: application/json' -X POST -d '{
  "contents": [{
    "parts":[{"text": "generate 60 sentences about current sports updates and write them line by line in plaintext and dont add numbering to the sentences. Each sports update sentence should be different in terms of content and shouldnot be too generic. Dont generate intro and outro text, just generate sentences, else no content"}]
    }]
   }' | jq -r '.candidates[0].content.parts[0].text' > sports.txt

curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=AIzaSyDSdd3ifQB0lBfsQVgqtiDSc6CiP57PDZM" -H 'Content-Type: application/json' -X POST -d '{
  "contents": [{
    "parts":[{"text": "generate 60 sentences about current stocks updates and write them line by line in plaintext and dont add numbering to the sentences. Each stocks update sentence should be different in terms of content and shouldnot be too generic. Dont generate intro and outro text, just generate sentences, else no content"}]
    }]
   }' | jq -r '.candidates[0].content.parts[0].text' > stocks.txt

curl "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=AIzaSyDSdd3ifQB0lBfsQVgqtiDSc6CiP57PDZM" -H 'Content-Type: application/json' -X POST -d '{
  "contents": [{
    "parts":[{"text": "generate 60 sentences about current crypto updates and write them line by line in plaintext and dont add numbering to the sentences. Each crypto update sentence should be different in terms of content and shouldnot be too generic. Dont generate intro and outro text, just generate sentences, else no content"}]
    }]
   }' | jq -r '.candidates[0].content.parts[0].text' > crypto.txt

