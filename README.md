# Dal's Wondering

> UE 5.4.4 기반 오픈월드 소울라이크 액션 RPG
> **Twiluna Studio · 4조**

---

## ✨ 프로젝트 한눈에 보기

| 항목         | 내용                                                                             |
| ---------- | ------------------------------------------------------------------------------ |
| **엔진**     | Unreal Engine 5.4.4                                                            |
| **장르**     | 싱글플레이 전용 3인칭 액션 RPG                                                            |
| **주요 기술**  | World Partition / Nanite / DLSS / Reflex / Detour Crowd + RVO / EQS            |
| **지원 플랫폼** | Windows 64bit (추가 플랫폼 지원 예정)                                                   |
| **개발 기간**  | 2025년 5월 7일 \~ 7월 1일 (약 2개월)                                                   |
| **핵심 시스템** | 콤보 전투 / 타이밍 패링 / 속성 무기 / 오픈월드 탐험 / 제작 및 강화 / 스킬 트리 / 인벤토리 시스템 / 동적 지도 / HUD 구성 |

---

## 🖥️ 요구 사양 *(고사양 권장)*

> *벤치마크 수치는 추후 업데이트 예정입니다.*

| 구분      | 최소                                           | 권장                      |
| ------- | -------------------------------------------- | ----------------------- |
| OS      | Windows 10 64-bit                            | Windows 11 64-bit       |
| CPU     | 6-core Intel / AMD                           | 8-core Intel / AMD      |
| GPU     | GTX 1660 Super                               | RTX 3060 Ti (DLSS 사용 시) |
| RAM     | 16 GB                                        | 32 GB                   |
| Storage | 30 GB SSD                                    | NVMe SSD                |
| IDE     | Rider 2024.1 / Visual Studio 2022 (최소·권장 동일) |                         |

---

## 🚀 빠른 시작 (로컬 빌드 가이드)

```bash
git clone <repo> DalWondering
cd DalWondering

# 1) Setup (필요 시)
./Setup.bat

# 2) 프로젝트 파일 생성
./GenerateProjectFiles.bat -game -vs2022

# 3) 빌드 및 실행
msbuild DalWondering.sln /t:Build /p:Configuration=Development
start DalWondering.uproject
```

※ `.uproject` 파일을 UE 런처에서 열고 Play-In-Editor 실행도 가능합니다. (패키징 미지원)

🎮 **기본 키 매핑**

| 액션                  | 키     | 액션                   | 키           |
| ------------------- | ----- | -------------------- | ----------- |
| Move Forward / Back | W / S | Skill 1 / 2 / 3      | 1 2 3       |
| Move Left / Right   | A / D | Ride / Interact      | G / E       |
| Jump                | Space | Guard                | RMB         |
| Attack              | LMB   | Dodge                | Left Shift  |
| Lock-on             | MMB   | Skill (패시브)          | R           |
| Pause               | ESC   | Camera Turn / LookUp | Mouse X / Y |

---

## 🔧 기술 아키텍처 및 시스템

### 🌍 오픈월드 스트리밍 & 최적화

* **World Partition + OFPA**: 대규모 월드를 셀 단위로 자동 분할/로드/언로드, 다수 인원 동시 편집 가능
* **HLOD & Nanite**: 원거리 오브젝트 자동 단순화, 메모리와 드로우콜 절감
* **Instance Mesh 사용**: 반복 오브젝트 최적화 렌더링

### 🎨 그래픽 렌더링 & 퍼포먼스

* **DLSS**: 해상도 업스케일링 및 프레임 생성으로 고해상도 퍼포먼스를 보장
* **Reflex**: 입력 지연 최소화 → 패링/회피 중심 전투에서 타이밍 정밀도 향상

### 🦾 캐릭터 모션 & 전투 메카닉

* 자연스러운 걷기/달리기/회피/방어, 락온 시스템
* 콤보 공격, 속성 무기 바인딩, 스태미너 소모 기반 전투, 타이밍 패링

### 🧠 AI & NPC 로직

* **Perception + EQS** 기반 적 감지 및 경로 탐색
* **Detour Crowd + RVO** 기반 군중 AI를 통해 마을이나 도시 환경에서 자연스럽게 이동하는 NPC 행동 구현

### ⚒️ 아이템 제작·강화·성장 시스템

* 몬스터 처치 → 숙련도 획득 → 스킬 트리 포인트 분배
* 강화 성공/실패에 따른 외형 변화 및 파괴 위험, 제작 재료 루프 설계
* **ItemCode 파싱 시스템**:
  “등급/강화/RowID” 정보를 문자열로 인코딩하여 아이템을 통합 관리. 코드 파싱 후 `ItemDataManager`에서 세부 데이터 로딩.

### 🖥️ UI/UX & HUD 구성

* 직관적인 HUD: 체력/기력/상태이상 등 필수 정보 시각화
* 인벤토리 UI: 아이템 분류, 드래그·상호작용, 정보창 연동
* 스킬 트리/제작/강화 메뉴 통합
* **동적 지도**: 플레이어가 지나간 경로만 표시되는 탐색형 맵 시스템

---

## 📂 폴더 구조 (상위 레벨 기준)

```
/Animations     ─ 캐릭터 및 몬스터 모션
/Character      ─ 플레이어 블루프린트 및 애님
/Interface      ─ 공통 인터페이스 (C++)
/Maps           ─ 월드 Partition 레벨 구성
/Monster        ─ 몬스터 AI 및 애셋
/NeutralityNPC  ─ 중립 NPC
/NPC            ─ 마을 NPC 및 상호작용
/Projectile     ─ 투사체 관련 로직
/Telegraph      ─ 공격 텔레그래프 비주얼 이펙트
/UI             ─ 위젯 및 HUD
/Core           ─ GameMode / GameInstance 등 전역 게임 흐름 관리
```

※ 추후 세부 폴더 구조 변경 가능

---

## 🧩 사용 플러그인 목록 (비기본 활성화 항목)

* **NVIDIA DLSS 관련**
  DLSS / StreamlineDLSSG / StreamlineReflex / StreamlineCore / DLSSMoviePipelineSupport
  → 고성능 업스케일링, 프레임 생성, Reflex 지연 최소화 지원

* **AsyncLoadingScreen**
  → 커스터마이징 가능한 로딩 화면 구현

* **PCG / Landmass / Water / Volumetrics**
  → 절차적 필드·던전 자동 생성 및 수계 지형 표현 강화

* **SkeletalMerging**
  → 파츠 교체형 캐릭터의 최적화 및 스켈레톤 병합 지원

---

## 🔀 브랜치 전략 & 커밋 규칙

| 브랜치        | 용도             |
| ---------- | -------------- |
| main       | 릴리즈용 안정 빌드     |
| dev        | 통합 테스트용 개발 브랜치 |
| feature/\* | 기능별 개발 브랜치     |

**커밋 태그 규칙**

* `[Add]` : 신규 기능, 시스템, 에셋 추가
* `[Fix]` : 버그 수정 또는 비효율 로직 개선
* `[Update]` : 기존 로직·시스템 개선
* `[Del]` : 불필요한 코드, 에셋 삭제
* `[Lock]`, `[Unlock]` : Git LFS 파일 관리용 Lock 메시지

---

## 🖋️ 코드 스타일 가이드

* Epic 공식 UE Coding Standard 기반

---

## 📜 라이선스 및 외부 에셋 출처

> 스토어 에셋 / 외부 리소스 / 사운드 / 폰트 등 항목별로 추후 정리 필요

---

## 🤝 Contributing

1. `feature/기능명` 브랜치 생성 후 작업
2. PR 생성 → CI 빌드 통과
3. 코드 리뷰 승인 → `dev` 머지
4. 스프린트 종료 시 `dev → main` 릴리즈 태그

---

## 📌 개발 지침 및 방향성

* 퀄리티 중심 챕터 완성 → 시스템 재활용 및 반복 작업 최소화
* 무료 에셋 적극 활용 및 기능 우선 개발
* Git 충돌 최소화를 위한 World Partition + 브랜치 전략 병행 운영
