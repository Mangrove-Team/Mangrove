import React, { useState, useRef, useEffect } from "react";
import {
  View,
  Text,
  StyleSheet,
  Modal,
  TouchableOpacity,
  Dimensions,
  PanResponder,
  Animated,
} from "react-native";

const { width: SCREEN_WIDTH, height: SCREEN_HEIGHT } = Dimensions.get("window");

const FLOOR_WIDTH = 900;
const FLOOR_HEIGHT = 620;

const STANDS = [
  { id: "A1", x: 60,  y: 80,  w: 110, h: 80, label: "A1", company: "Université Paris-Saclay", sector: "Enseignement supérieur", contact: "contact@paris-saclay.fr", color: "#4A90D9",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: false },
    equipment: ["2 tables", "4 chaises", "1 kakémono", "1 présentoir brochures"],
    notes: "Arrivée souhaitée dès 7h30 pour installation." },

  { id: "A2", x: 200, y: 80,  w: 110, h: 80, label: "A2", company: "École Polytechnique", sector: "Grande école", contact: "admissions@polytechnique.edu", color: "#4A90D9",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["1 table", "2 chaises", "1 écran 32\"", "1 kakémono"],
    notes: "Nécessite un accès au courant 220V (2 prises min)." },

  { id: "A3", x: 340, y: 80,  w: 110, h: 80, label: "A3", company: "HEC Paris", sector: "École de commerce", contact: "info@hec.fr", color: "#4A90D9",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: false, lighting: false },
    equipment: ["2 tables", "3 chaises", "1 kakémono"],
    notes: "Stand partagé avec HEC Executive Education." },

  { id: "A4", x: 480, y: 80,  w: 110, h: 80, label: "A4", company: "Sciences Po", sector: "Enseignement supérieur", contact: "info@sciencespo.fr", color: "#4A90D9",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: false },
    equipment: ["1 table", "4 chaises", "1 tablette tactile", "1 kakémono"],
    notes: "" },

  { id: "A5", x: 620, y: 80,  w: 110, h: 80, label: "A5", company: "CentraleSupélec", sector: "Grande école", contact: "admissions@centralesupelec.fr", color: "#4A90D9",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["1 table", "2 chaises", "1 écran 27\"", "1 kakémono"],
    notes: "Démonstration robotique prévue — espace dégagé nécessaire." },

  { id: "A6", x: 760, y: 80,  w: 100, h: 80, label: "A6", company: "ESCP Business School", sector: "École de commerce", contact: "mim@escp.eu", color: "#4A90D9",
    size: { width: 2.5, depth: 2 }, unit: "m",
    constraints: { electricity: false, water: false, internet: false, lighting: false },
    equipment: ["1 table", "2 chaises", "1 kakémono"],
    notes: "" },

  { id: "B1", x: 60,  y: 230, w: 110, h: 80, label: "B1", company: "Google France", sector: "Tech & Numérique", contact: "campus@google.com", color: "#34A853",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["2 tables hautes", "4 tabourets", "2 écrans 32\"", "1 kakémono"],
    notes: "Câblage réseau RJ45 requis (Wi-Fi insuffisant pour la démo)." },

  { id: "B2", x: 200, y: 230, w: 110, h: 80, label: "B2", company: "BNP Paribas", sector: "Finance & Banque", contact: "graduates@bnpparibas.com", color: "#34A853",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: false },
    equipment: ["2 tables", "4 chaises", "1 kakémono", "1 présentoir"],
    notes: "" },

  { id: "B3", x: 340, y: 230, w: 110, h: 80, label: "B3", company: "Airbus", sector: "Aéronautique", contact: "recruitment@airbus.com", color: "#34A853",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["1 table", "2 chaises", "1 écran 42\"", "1 maquette avion"],
    notes: "Maquette fragile — prévoir espace sécurisé. 3 prises 220V requises." },

  { id: "B4", x: 480, y: 230, w: 110, h: 80, label: "B4", company: "McKinsey & Co.", sector: "Conseil", contact: "paris@mckinsey.com", color: "#34A853",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: false },
    equipment: ["1 table ronde", "4 chaises", "1 kakémono"],
    notes: "Préfère un emplacement calme (entretiens informels)." },

  { id: "B5", x: 620, y: 230, w: 110, h: 80, label: "B5", company: "Sanofi", sector: "Santé & Pharma", contact: "campus@sanofi.com", color: "#34A853",
    size: { width: 3, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: true, internet: false, lighting: false },
    equipment: ["2 tables", "3 chaises", "1 kakémono", "1 évier portable"],
    notes: "Accès eau potable requis (démo produit). Prévoir évacuation." },

  { id: "B6", x: 760, y: 230, w: 100, h: 80, label: "B6", company: "EDF", sector: "Énergie", contact: "recrutement@edf.fr", color: "#34A853",
    size: { width: 2.5, depth: 2 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["1 table", "2 chaises", "1 écran 32\"", "1 kakémono"],
    notes: "" },

  { id: "C1", x: 60,  y: 390, w: 170, h: 90, label: "C1", company: "INRIA", sector: "Recherche", contact: "recrutement@inria.fr", color: "#9B59B6",
    size: { width: 4, depth: 3 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["3 tables", "6 chaises", "2 écrans 27\"", "1 kakémono", "1 serveur portable"],
    notes: "Démo IA interactive — bande passante élevée requise (min 50 Mbps)." },

  { id: "C2", x: 270, y: 390, w: 170, h: 90, label: "C2", company: "Capgemini", sector: "Tech & Conseil", contact: "campus@capgemini.com", color: "#9B59B6",
    size: { width: 4, depth: 3 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: false },
    equipment: ["2 tables hautes", "4 tabourets", "1 écran 55\"", "2 kakémonos"],
    notes: "Écran TV à fixer sur pied — prévoir stabilisation." },

  { id: "C3", x: 480, y: 390, w: 170, h: 90, label: "C3", company: "LVMH", sector: "Luxe & Mode", contact: "talents@lvmh.com", color: "#9B59B6",
    size: { width: 4, depth: 3 }, unit: "m",
    constraints: { electricity: true, water: false, internet: false, lighting: true },
    equipment: ["1 table basse", "4 fauteuils", "2 kakémonos", "Éclairage spots LED"],
    notes: "Éclairage personnalisé apporté par le stand — prévoir 4 prises 220V." },

  { id: "C4", x: 690, y: 390, w: 170, h: 90, label: "C4", company: "Total Energies", sector: "Énergie", contact: "campus@totalenergies.com", color: "#9B59B6",
    size: { width: 4, depth: 3 }, unit: "m",
    constraints: { electricity: true, water: false, internet: true, lighting: true },
    equipment: ["2 tables", "4 chaises", "1 écran 42\"", "1 kakémono", "1 borne interactive"],
    notes: "Borne interactive nécessite connexion internet stable. 4 prises requises." },
];

const SECTOR_COLORS = {
  "Enseignement supérieur": "#4A90D9",
  "Grande école": "#4A90D9",
  "École de commerce": "#4A90D9",
  "Tech & Numérique": "#34A853",
  "Finance & Banque": "#34A853",
  "Aéronautique": "#34A853",
  "Conseil": "#34A853",
  "Santé & Pharma": "#34A853",
  "Énergie": "#34A853",
  "Recherche": "#9B59B6",
  "Tech & Conseil": "#9B59B6",
  "Luxe & Mode": "#9B59B6",
};

const LEGEND = [
  { color: "#4A90D9", label: "Formations" },
  { color: "#34A853", label: "Entreprises" },
  { color: "#9B59B6", label: "Recherche & Grands groupes" },
];

export default function FloorPlanScreen() {
  const [selectedStand, setSelectedStand] = useState(null);
  const [highlightedStand, setHighlightedStand] = useState(null);
  const [scale, setScale] = useState(0.72);
  const [offset, setOffset] = useState({ x: 0, y: 0 });
  const pulseAnim = useRef(new Animated.Value(1)).current;

  useEffect(() => {
    if (highlightedStand) {
      Animated.loop(
        Animated.sequence([
          Animated.timing(pulseAnim, { toValue: 1.08, duration: 400, useNativeDriver: true }),
          Animated.timing(pulseAnim, { toValue: 1, duration: 400, useNativeDriver: true }),
        ])
      ).start();
      const timer = setTimeout(() => {
        pulseAnim.stopAnimation();
        pulseAnim.setValue(1);
        setHighlightedStand(null);
      }, 3000);
      return () => clearTimeout(timer);
    }
  }, [highlightedStand]);

  const offsetRef = useRef({ x: 0, y: 0 });
  const scaleRef = useRef(0.72);
  const lastOffset = useRef({ x: 0, y: 0 });
  const lastDistance = useRef(null);
  const lastScale = useRef(0.72);

  const panResponder = useRef(
    PanResponder.create({
      onStartShouldSetPanResponder: () => true,
      onMoveShouldSetPanResponder: () => true,

      onPanResponderGrant: (evt) => {
        lastOffset.current = { ...offsetRef.current };
        lastScale.current = scaleRef.current;
        const touches = evt.nativeEvent.touches;
        if (touches.length === 2) {
          const dx = touches[0].pageX - touches[1].pageX;
          const dy = touches[0].pageY - touches[1].pageY;
          lastDistance.current = Math.sqrt(dx * dx + dy * dy);
        } else {
          lastDistance.current = null;
        }
      },

      onPanResponderMove: (evt, gestureState) => {
        const touches = evt.nativeEvent.touches;

        if (touches.length === 2) {
          const dx = touches[0].pageX - touches[1].pageX;
          const dy = touches[0].pageY - touches[1].pageY;
          const distance = Math.sqrt(dx * dx + dy * dy);

          if (lastDistance.current === null) {
            lastDistance.current = distance;
            lastScale.current = scaleRef.current;
            return;
          }

          const ratio = distance / lastDistance.current;
          const newScale = Math.min(3, Math.max(0.4, lastScale.current * ratio));
          scaleRef.current = newScale;
          setScale(newScale);
        } else {
          lastDistance.current = null;
          const newOffset = {
            x: lastOffset.current.x + gestureState.dx,
            y: lastOffset.current.y + gestureState.dy,
          };
          offsetRef.current = newOffset;
          setOffset(newOffset);
        }
      },

      onPanResponderRelease: () => {
        lastDistance.current = null;
      },
    })
  ).current;

  const handleStandPress = (stand) => {
    setSelectedStand(stand);
  };

  return (
    <View style={styles.container}>
      <View style={styles.header}>
        <Text style={styles.headerTitle}>🎓 Salon de l'Étudiant 2025</Text>
        <Text style={styles.headerSub}>Pincer pour zoomer · Glisser pour naviguer</Text>
      </View>

      <View style={styles.legend}>
        {LEGEND.map((item) => (
          <View key={item.label} style={styles.legendItem}>
            <View style={[styles.legendDot, { backgroundColor: item.color }]} />
            <Text style={styles.legendText}>{item.label}</Text>
          </View>
        ))}
      </View>

      <View style={styles.mapContainer} {...panResponder.panHandlers}>
        <View
          style={[
            styles.floorPlan,
            {
              width: FLOOR_WIDTH,
              height: FLOOR_HEIGHT,
              transform: [
                { translateX: offset.x },
                { translateY: offset.y },
                { scale: scale },
              ],
            },
          ]}
        >
          <Text style={[styles.rowLabel, { top: 112, left: 12 }]}>A</Text>
          <Text style={[styles.rowLabel, { top: 262, left: 12 }]}>B</Text>
          <Text style={[styles.rowLabel, { top: 420, left: 12 }]}>C</Text>

          <View style={styles.entrance}>
            <Text style={styles.entranceText}>🚪 ENTRÉE</Text>
          </View>

          <View style={[styles.aisle, { top: 175, left: 50, width: FLOOR_WIDTH - 60 }]} />
          <View style={[styles.aisle, { top: 335, left: 50, width: FLOOR_WIDTH - 60 }]} />

          {STANDS.map((stand) => {
            const isHighlighted = highlightedStand === stand.id;
            return (
              <Animated.View
                key={stand.id}
                style={[
                  styles.stand,
                  {
                    left: stand.x,
                    top: stand.y,
                    width: stand.w,
                    height: stand.h,
                    backgroundColor: isHighlighted ? stand.color + "66" : stand.color + "22",
                    borderColor: stand.color,
                    borderWidth: isHighlighted ? 3 : 1.5,
                    shadowColor: stand.color,
                    shadowOpacity: isHighlighted ? 1 : 0,
                    shadowRadius: isHighlighted ? 16 : 0,
                    elevation: isHighlighted ? 12 : 0,
                    transform: [{ scale: isHighlighted ? pulseAnim : 1 }],
                    zIndex: isHighlighted ? 10 : 1,
                  },
                ]}
              >
                <TouchableOpacity
                  style={{ flex: 1, justifyContent: "center", alignItems: "center" }}
                  onPress={() => handleStandPress(stand)}
                  activeOpacity={0.75}
                >
                  <Text style={[styles.standLabel, { color: stand.color }]}>{stand.label}</Text>
                  <Text style={styles.standName} numberOfLines={2}>{stand.company}</Text>
                </TouchableOpacity>
              </Animated.View>
            );
          })}
        </View>
      </View>

      <TouchableOpacity
        style={styles.resetButton}
        onPress={() => { scaleRef.current = 0.72; offsetRef.current = { x: 0, y: 0 }; setScale(0.72); setOffset({ x: 0, y: 0 }); }}
      >
        <Text style={styles.resetText}>⟳ Réinitialiser</Text>
      </TouchableOpacity>

      <Modal
        visible={!!selectedStand}
        transparent
        animationType="slide"
        onRequestClose={() => setSelectedStand(null)}
      >
        <View style={styles.modalOverlay}>
          <View style={styles.modalCard}>
            {selectedStand && (
              <>
                <View style={[styles.modalAccent, { backgroundColor: selectedStand.color }]} />
                <TouchableOpacity style={styles.closeBtn} onPress={() => setSelectedStand(null)}>
                  <Text style={styles.closeBtnText}>✕</Text>
                </TouchableOpacity>

                <View style={styles.modalHeader}>
                  <View style={[styles.modalLabelBadge, { backgroundColor: selectedStand.color }]}>
                    <Text style={styles.modalLabelBadgeText}>{selectedStand.label}</Text>
                  </View>
                  <View style={{ flex: 1 }}>
                    <Text style={styles.modalCompany}>{selectedStand.company}</Text>
                    <View style={[styles.sectorBadge, { backgroundColor: selectedStand.color + "22", borderColor: selectedStand.color }]}>
                      <Text style={[styles.sectorText, { color: selectedStand.color }]}>{selectedStand.sector}</Text>
                    </View>
                  </View>
                </View>

                <View style={styles.modalScroll}>

                  <Text style={styles.modalSectionTitle}>📐 Dimensions du stand</Text>
                  <View style={styles.infoRow}>
                    <View style={styles.infoBox}>
                      <Text style={styles.infoBoxValue}>{selectedStand.size.width} {selectedStand.unit}</Text>
                      <Text style={styles.infoBoxLabel}>Largeur</Text>
                    </View>
                    <View style={styles.infoBox}>
                      <Text style={styles.infoBoxValue}>{selectedStand.size.depth} {selectedStand.unit}</Text>
                      <Text style={styles.infoBoxLabel}>Profondeur</Text>
                    </View>
                    <View style={styles.infoBox}>
                      <Text style={styles.infoBoxValue}>{selectedStand.size.width * selectedStand.size.depth} m²</Text>
                      <Text style={styles.infoBoxLabel}>Surface</Text>
                    </View>
                  </View>

                  <Text style={styles.modalSectionTitle}>⚡ Contraintes techniques</Text>
                  <View style={styles.constraintsRow}>
                    <View style={[styles.constraintChip, selectedStand.constraints.electricity ? styles.chipOn : styles.chipOff]}>
                      <Text style={styles.constraintIcon}>⚡</Text>
                      <Text style={[styles.constraintLabel, selectedStand.constraints.electricity ? styles.chipTextOn : styles.chipTextOff]}>Électricité</Text>
                    </View>
                    <View style={[styles.constraintChip, selectedStand.constraints.water ? styles.chipOn : styles.chipOff]}>
                      <Text style={styles.constraintIcon}>💧</Text>
                      <Text style={[styles.constraintLabel, selectedStand.constraints.water ? styles.chipTextOn : styles.chipTextOff]}>Eau</Text>
                    </View>
                    <View style={[styles.constraintChip, selectedStand.constraints.internet ? styles.chipOn : styles.chipOff]}>
                      <Text style={styles.constraintIcon}>🌐</Text>
                      <Text style={[styles.constraintLabel, selectedStand.constraints.internet ? styles.chipTextOn : styles.chipTextOff]}>Internet</Text>
                    </View>
                    <View style={[styles.constraintChip, selectedStand.constraints.lighting ? styles.chipOn : styles.chipOff]}>
                      <Text style={styles.constraintIcon}>💡</Text>
                      <Text style={[styles.constraintLabel, selectedStand.constraints.lighting ? styles.chipTextOn : styles.chipTextOff]}>Éclairage</Text>
                    </View>
                  </View>

                  <Text style={styles.modalSectionTitle}>🪑 Équipements demandés</Text>
                  <View style={styles.equipmentList}>
                    {selectedStand.equipment.map((item, i) => (
                      <View key={i} style={styles.equipmentItem}>
                        <View style={[styles.equipmentDot, { backgroundColor: selectedStand.color }]} />
                        <Text style={styles.equipmentText}>{item}</Text>
                      </View>
                    ))}
                  </View>

                  <Text style={styles.modalSectionTitle}>✉️ Contact</Text>
                  <Text style={styles.modalContact}>{selectedStand.contact}</Text>

                  {selectedStand.notes ? (
                    <>
                      <Text style={styles.modalSectionTitle}>📝 Notes organisateur</Text>
                      <View style={styles.notesBox}>
                        <Text style={styles.notesText}>{selectedStand.notes}</Text>
                      </View>
                    </>
                  ) : null}

                </View>

                <TouchableOpacity
                  style={[styles.ctaButton, { backgroundColor: selectedStand.color }]}
                  onPress={() => {
                    setHighlightedStand(selectedStand.id);
                    setSelectedStand(null);
                  }}
                >
                  <Text style={styles.ctaText}>📍 Localiser sur le plan</Text>
                </TouchableOpacity>
              </>
            )}
          </View>
        </View>
      </Modal>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#0F1117",
  },
  header: {
    paddingTop: 54,
    paddingHorizontal: 20,
    paddingBottom: 12,
    backgroundColor: "#0F1117",
  },
  headerTitle: {
    fontSize: 20,
    fontWeight: "700",
    color: "#FFFFFF",
    letterSpacing: 0.3,
  },
  headerSub: {
    fontSize: 12,
    color: "#666",
    marginTop: 2,
  },
  legend: {
    flexDirection: "row",
    paddingHorizontal: 20,
    paddingBottom: 10,
    gap: 16,
    flexWrap: "wrap",
  },
  legendItem: {
    flexDirection: "row",
    alignItems: "center",
    gap: 6,
  },
  legendDot: {
    width: 10,
    height: 10,
    borderRadius: 5,
  },
  legendText: {
    fontSize: 11,
    color: "#AAA",
  },
  mapContainer: {
    flex: 1,
    overflow: "hidden",
    backgroundColor: "#161820",
    margin: 12,
    borderRadius: 16,
    borderWidth: 1,
    borderColor: "#2A2D3A",
  },
  floorPlan: {
    position: "absolute",
    backgroundColor: "#1C1F2E",
    borderRadius: 12,
  },
  rowLabel: {
    position: "absolute",
    color: "#444",
    fontSize: 28,
    fontWeight: "800",
    zIndex: 0,
  },
  entrance: {
    position: "absolute",
    bottom: 18,
    left: FLOOR_WIDTH / 2 - 60,
    backgroundColor: "#2A2D3A",
    paddingHorizontal: 16,
    paddingVertical: 6,
    borderRadius: 20,
    borderWidth: 1,
    borderColor: "#3A3D4A",
  },
  entranceText: {
    color: "#888",
    fontSize: 12,
    fontWeight: "600",
  },
  aisle: {
    position: "absolute",
    height: 1,
    backgroundColor: "#2A2D3A",
  },
  stand: {
    position: "absolute",
    borderWidth: 1.5,
    borderRadius: 10,
    padding: 8,
    justifyContent: "center",
    alignItems: "center",
  },
  standLabel: {
    fontSize: 11,
    fontWeight: "800",
    letterSpacing: 0.5,
    marginBottom: 4,
  },
  standName: {
    fontSize: 10,
    color: "#CCC",
    textAlign: "center",
    lineHeight: 13,
  },
  resetButton: {
    alignSelf: "center",
    marginVertical: 10,
    backgroundColor: "#2A2D3A",
    paddingHorizontal: 20,
    paddingVertical: 8,
    borderRadius: 20,
  },
  resetText: {
    color: "#AAA",
    fontSize: 13,
    fontWeight: "600",
  },

  modalOverlay: {
    flex: 1,
    justifyContent: "flex-end",
    backgroundColor: "rgba(0,0,0,0.6)",
  },
  modalCard: {
    backgroundColor: "#1C1F2E",
    borderTopLeftRadius: 24,
    borderTopRightRadius: 24,
    padding: 24,
    paddingBottom: 40,
    maxHeight: "85%",
  },
  modalAccent: {
    width: 40,
    height: 4,
    borderRadius: 2,
    alignSelf: "center",
    marginBottom: 16,
  },
  closeBtn: {
    position: "absolute",
    top: 20,
    right: 20,
    width: 32,
    height: 32,
    borderRadius: 16,
    backgroundColor: "#2A2D3A",
    justifyContent: "center",
    alignItems: "center",
    zIndex: 10,
  },
  closeBtnText: {
    color: "#888",
    fontSize: 14,
  },
  modalHeader: {
    flexDirection: "row",
    alignItems: "flex-start",
    gap: 14,
    marginBottom: 20,
  },
  modalLabelBadge: {
    width: 44,
    height: 44,
    borderRadius: 12,
    justifyContent: "center",
    alignItems: "center",
  },
  modalLabelBadgeText: {
    color: "#FFF",
    fontWeight: "800",
    fontSize: 15,
  },
  modalCompany: {
    fontSize: 17,
    fontWeight: "800",
    color: "#FFF",
    marginBottom: 6,
  },
  sectorBadge: {
    alignSelf: "flex-start",
    paddingHorizontal: 10,
    paddingVertical: 3,
    borderRadius: 20,
    borderWidth: 1,
  },
  sectorText: {
    fontSize: 11,
    fontWeight: "600",
  },
  modalScroll: {
    marginBottom: 16,
  },
  modalSectionTitle: {
    fontSize: 11,
    fontWeight: "700",
    color: "#555",
    letterSpacing: 0.8,
    textTransform: "uppercase",
    marginBottom: 8,
    marginTop: 16,
  },
  infoRow: {
    flexDirection: "row",
    gap: 10,
  },
  infoBox: {
    flex: 1,
    backgroundColor: "#252836",
    borderRadius: 12,
    paddingVertical: 10,
    alignItems: "center",
  },
  infoBoxValue: {
    fontSize: 16,
    fontWeight: "800",
    color: "#FFF",
  },
  infoBoxLabel: {
    fontSize: 10,
    color: "#666",
    marginTop: 2,
  },
  constraintsRow: {
    flexDirection: "row",
    gap: 8,
    flexWrap: "wrap",
  },
  constraintChip: {
    flexDirection: "row",
    alignItems: "center",
    gap: 5,
    paddingHorizontal: 12,
    paddingVertical: 7,
    borderRadius: 20,
    borderWidth: 1,
  },
  chipOn: {
    backgroundColor: "#1E3A2A",
    borderColor: "#34A853",
  },
  chipOff: {
    backgroundColor: "#252836",
    borderColor: "#333",
  },
  constraintIcon: {
    fontSize: 13,
  },
  constraintLabel: {
    fontSize: 12,
    fontWeight: "600",
  },
  chipTextOn: {
    color: "#34A853",
  },
  chipTextOff: {
    color: "#444",
  },
  equipmentList: {
    gap: 6,
  },
  equipmentItem: {
    flexDirection: "row",
    alignItems: "center",
    gap: 8,
  },
  equipmentDot: {
    width: 6,
    height: 6,
    borderRadius: 3,
  },
  equipmentText: {
    fontSize: 13,
    color: "#CCC",
  },
  modalContact: {
    fontSize: 13,
    color: "#AAA",
  },
  notesBox: {
    backgroundColor: "#252836",
    borderRadius: 12,
    padding: 12,
    borderLeftWidth: 3,
    borderLeftColor: "#F5A623",
  },
  notesText: {
    fontSize: 13,
    color: "#CCC",
    lineHeight: 19,
  },
  ctaButton: {
    paddingVertical: 14,
    borderRadius: 14,
    alignItems: "center",
  },
  ctaText: {
    color: "#FFF",
    fontWeight: "700",
    fontSize: 15,
  },
});