# Distortion
- 엔진 버전 : UE 5.4.4
- 장르 : 멀티플레이 기반 액션 장르

특징
1. UI MVVM 패턴 구현

2. 세밀한 근접 충돌 처리
- 틱이 아닌 타이머 처리, FPS에 영향을 덜 받음
- 현재 프레임과 이전 프레임을 기반으로 보간 및 베지어 곡선을 활용한 세밀한 판정
(언리얼 페스트 2024, 시프트업 자료 참고 https://youtu.be/IL9j4NchTvA?si=vEgCtYrbNi8DaMOV)

3. 오브젝트 풀링 사용
- UWorldSubsystem를 활용한 투사체 풀링 적용
- 서버에서 호출해주어야 다른 클라에서도 적용
